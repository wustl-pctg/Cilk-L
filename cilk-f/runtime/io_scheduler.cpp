#include "io_scheduler.h"

#include <cilk/future.h>
#include <cilk/cilk_io.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <unordered_map>
#include <unordered_set>
#include <deque>

#include "bug.h"
#include "global_state.h"
#include "local_state.h"
#include "os.h"

extern "C" {

void make_worker_io(__cilkrts_worker *w) {
    CILK_ASSERT(WORKER_FREE == w->l->type);
    w->l->type = WORKER_IO;
    w->l->io_queue = new_io_queue();
}

static int perform_io_until_block(const int &syscall_no, io_op_t &op) {
    int res;
    int orig_nbytes = op.nbyte;
    do {
      res = syscall(syscall_no, op.fildes, op.buf, op.nbyte);
      op.nbyte -= res;
      op.res += res;
      op.buf = (void*)((char*)op.buf + res);
    } while (op.nbyte > 0 && res > 0 && syscall_no != SYS_read);

    if (res < 0) {
      op.nbyte += res;
      op.buf = (void*)((char*)op.buf - res);
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        op.res -= res;
        return 1;
      } else {
        op.res = -1;
      }
    }

    io_future_result fut_val = {
      .ret_val = op.res,
      .errno_val = errno
    };

    void *deque = ((base_io_fut*)op.fut.f)->put(std::move(fut_val));
    if (deque) __cilkrts_make_resumable(deque);

    return 0;
}

static void __attribute__((always_inline)) handle_event(const int &syscall_no, const int &fd, std::deque<io_op_t> &op_queue, std::unordered_set<int> &blocked_set, std::unordered_set<int> &unblocked_set) {
  int res = 0;
  while (!op_queue.empty() && res != 1) {
    io_op_t& op = op_queue.front();
    res = perform_io_until_block(syscall_no, op);
    if (res == 0) {
        op_queue.pop_front();
    }
  }
  if (op_queue.empty()) {
    blocked_set.erase(fd);
    unblocked_set.emplace(fd);
  }
}

void scheduler_thread_proc_for_io_worker(void* arg) {
  __cilkrts_worker *self = (__cilkrts_worker*)arg;
  __cilkrts_set_tls_worker(self);

  __cilkrts_worker *coworker = self->g->workers[self->self - self->g->P];

  io_op_t curr_op;

  int epoll_fd = epoll_create(4096); // Argument just needs to be positive

  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.data.fd = self->l->io_queue->eventfd;
  event.events = EPOLLIN | EPOLLET;

  int num_triggered = 0;
  struct epoll_event triggered_events[4096];
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, self->l->io_queue->eventfd, &event);
  ssize_t ret_val;

  // The first deque contains read ops, the second deque contains write ops
  std::unordered_map<int, std::pair<std::deque<io_op_t>, std::deque<io_op_t> > > blocked_ops;

  std::unordered_set<int> blocked_read_fds;
  std::unordered_set<int> ready_read_fds;
  std::unordered_set<int> blocked_write_fds;
  std::unordered_set<int> ready_write_fds;
  bool check_queue = true;

  while (!self->g->work_done) {

      num_triggered = epoll_wait(epoll_fd, triggered_events, 4096, -1);
      if (num_triggered < 0) {
          __cilkrts_bug("epoll_wait returned an error (%s)\n", strerror(errno));
      }
      for (int i = 0; i < num_triggered; i++) {
          if (triggered_events[i].data.fd == self->l->io_queue->eventfd) {
            check_queue = true;
            continue;
          }
          if (triggered_events[i].events & EPOLLIN) {
              int fd = triggered_events[i].data.fd;
              handle_event(SYS_read, fd, blocked_ops[fd].first, blocked_read_fds, ready_read_fds);
          }
          if (triggered_events[i].events & EPOLLOUT) {
              int fd = triggered_events[i].data.fd;
              handle_event(SYS_write, fd, blocked_ops[fd].second, blocked_write_fds, ready_write_fds);
          }
      }

    if (check_queue) {
        check_queue = false;
        //int val = 0;
        //syscall(SYS_read, self->l->io_queue->eventfd, &val, 8);

        do {
            curr_op = io_queue_pop(self->l->io_queue);
            if (curr_op.type == IOTYPE__READ) {
                if (blocked_read_fds.count(curr_op.fildes)) {
                    blocked_ops[curr_op.fildes].first.emplace_back(std::move(curr_op));
                } else {
                    ret_val = perform_io_until_block(SYS_read, curr_op);
                    // If we would have blocked
                    if (ret_val) {
                        if (ready_read_fds.count(curr_op.fildes)) {
                            blocked_read_fds.emplace(curr_op.fildes);
                            ready_read_fds.erase(curr_op.fildes);
                            blocked_ops[curr_op.fildes].first.emplace_back(std::move(curr_op));
                        } else {
                            blocked_read_fds.emplace(curr_op.fildes);
                            blocked_ops[curr_op.fildes].first.emplace_back(std::move(curr_op));
                            event = {
                                .data.fd = curr_op.fildes,
                                .events = EPOLLIN | EPOLLET
                            };
                            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, curr_op.fildes, &event);
                        }
                    }
                }
            } else if (curr_op.type == IOTYPE__WRITE) {
                if (blocked_write_fds.count(curr_op.fildes)) {
                    blocked_ops[curr_op.fildes].second.emplace_back(std::move(curr_op));
                } else {
                    ret_val = perform_io_until_block(SYS_write, curr_op);
                    // If we would have blocked
                    if (ret_val) {
                        if (ready_write_fds.count(curr_op.fildes)) {
                            blocked_write_fds.emplace(curr_op.fildes);
                            ready_write_fds.erase(curr_op.fildes);
                            blocked_ops[curr_op.fildes].second.emplace_back(std::move(curr_op));
                        } else {
                            blocked_write_fds.emplace(curr_op.fildes);
                            blocked_ops[curr_op.fildes].second.emplace_back(std::move(curr_op));
                            event = {
                                .data.fd = curr_op.fildes,
                                .events = EPOLLOUT | EPOLLET
                            };
                            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, curr_op.fildes, &event);
                        }
                    }
                }
            }
        } while (curr_op.type != IOTYPE__INVALID);
    }

    for (int fd : ready_read_fds) {
      event = {
          .data.fd = fd,
          .events = EPOLLIN
      };
      epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
    }
    ready_read_fds.clear();

    for (int fd : ready_write_fds) {
      event = {
          .data.fd = fd,
          .events = EPOLLOUT
      };
      epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
    }
    ready_write_fds.clear();

  } // while(!self->g->work_done)

}

}
