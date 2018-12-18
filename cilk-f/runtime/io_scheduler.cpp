#include "io_scheduler.h"

#include <cilk/future.h>
#include <cilk/cilk_io.h>
#include <internal/abi.h>
#include <unistd.h>

#include "global_state.h"
#include "local_state.h"
#include "os.h"

extern "C" {

void scheduler_thread_proc_for_io_worker(void* arg) {
  printf("Ran an io worker thread!\n");

  __cilkrts_worker *self = (__cilkrts_worker*)arg;
  __cilkrts_set_tls_worker(self);

  __cilkrts_worker *coworker = self->g->workers[self->self - self->g->P];

  io_op_t curr_op;
  void *deque = NULL;
  do {
    curr_op = io_queue_pop(self->l->io_queue);
    switch(curr_op.type) {
      case IOTYPE__READ:
        {
          deque = ((base_io_fut*)curr_op.fut.f)->put(read(curr_op.fildes, curr_op.buf, curr_op.nbyte));
        }
        break;

      case IOTYPE__WRITE:
        {
          deque = ((base_io_fut*)curr_op.fut.f)->put(write(curr_op.fildes, curr_op.buf, curr_op.nbyte));
        }
        break;

      case IOTYPE__INVALID:
      case IOTYPE__QUIT:
        break;

      default:
        __cilkrts_bug("ERR: Invalid IO type passed to IO worker! (%d)\n", curr_op.type);
        CILK_ASSERT(!"Should not arrive here");
        break; // Technically this shouldn't ever execute
    }

    if (deque) __cilkrts_make_resumable(deque);
    deque = NULL;
  } while (curr_op.type != IOTYPE__QUIT);
}

}
