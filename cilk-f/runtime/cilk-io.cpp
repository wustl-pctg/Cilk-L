#include <cilk/cilk_io.h>
#include <cilk/future.h>
#include <unistd.h>

#include "cilk-io-queue.h"
#include "local_state.h"

extern "C" {

io_future cilk_read(int fildes, void *buf, size_t nbyte) {
  io_future fut;
  fut.f = (void*) new base_io_fut();

  //__cilkrts_worker *self = __cilkrts_get_tls_worker_fast();

  ((base_io_fut*)fut.f)->put(read(fildes, buf, nbyte));

  return fut;
}

io_future cilk_write(int fildes, void *buf, size_t nbyte) {
  io_future fut;
  fut.f = (void*) new base_io_fut();

  __cilkrts_worker *self = __cilkrts_get_tls_worker_fast();
  if (self->l->io_queue) {
    io_op_t op = {
      .type = IOTYPE__WRITE,
      .fildes = fildes,
      .buf = buf,
      .nbyte = nbyte,
      .fut = fut
    };

    io_queue_push(self->l->io_queue, &op);
  } else {
    ((base_io_fut*)fut.f)->put(write(fildes, buf, nbyte));
  }

  return fut;
}

ssize_t cilk_iosync(io_future *fut) {
  ssize_t ret = ((base_io_fut*)fut->f)->get();

  delete (base_io_fut*)(fut->f);
  fut->f = NULL;

  return ret;
}

}
