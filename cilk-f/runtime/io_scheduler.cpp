#include "io_scheduler.h"

#include <cilk/future.h>
#include <cilk/cilk_io.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "bug.h"
#include "global_state.h"
#include "local_state.h"
#include "os.h"

extern "C" {

void make_worker_io(__cilkrts_worker *w) {
    CILK_ASSERT(WORKER_FREE == w->l->type);
    w->l->type = WORKER_IO;
    w->l->io_queue = new_io_queue();
    //w->l->signal_node = signal_node_create();
}

void scheduler_thread_proc_for_io_worker(void* arg) {
  printf("Ran an io worker thread!\n");

  __cilkrts_worker *self = (__cilkrts_worker*)arg;
  __cilkrts_set_tls_worker(self);

  __cilkrts_worker *coworker = self->g->workers[self->self - self->g->P];

  io_op_t curr_op;
  void *deque = NULL;

  int epoll_fd = epoll_create(4096); // Argument just needs to be positive
  struct epoll_event event = {
    .data.fd = self->l->io_queue->eventfd,
    .events = EPOLLIN | EPOLLET
  };
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, self->l->io_queue->eventfd, &event);

  while (!self->g->work_done) {
    do {

      curr_op = io_queue_pop(self->l->io_queue);

      switch(curr_op.type) {
        case IOTYPE__READ:
          {
            deque = ((base_io_fut*)curr_op.fut.f)->put(syscall(SYS_read, curr_op.fildes, curr_op.buf, curr_op.nbyte));
          }
          break;

        case IOTYPE__WRITE:
          {
            deque = ((base_io_fut*)curr_op.fut.f)->put(syscall(SYS_write, curr_op.fildes, curr_op.buf, curr_op.nbyte));
          }
          break;

        case IOTYPE__INVALID:
        case IOTYPE__QUIT:
          break;

        default:
          __cilkrts_bug("ERR: Invalid IO type passed to IO worker! (%d)\n", curr_op.type);
          CILK_ASSERT(!"Should not arrive here");
          break;

      } // switch(curr_op.type)

      if (deque) __cilkrts_make_resumable(deque);
      deque = NULL;

    } while (curr_op.type != IOTYPE__QUIT && curr_op.type != IOTYPE__INVALID);

    if (curr_op.type == IOTYPE__INVALID) {
      epoll_wait(epoll_fd, &event, 1, -1);
    }

  } // while(!self->g->work_done)

}

}
