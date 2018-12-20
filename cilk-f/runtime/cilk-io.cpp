#include <cilk/cilk_io.h>
#include <cilk/future.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

#include "cilk-io-queue.h"
#include "local_state.h"

extern "C" {


io_future cilk_read(int fildes, void *buf, size_t nbyte) {
  io_future fut;
  fut.f = (void*) new base_io_fut();

  __cilkrts_worker *self = __cilkrts_get_tls_worker_fast();
  if (self->l->io_queue) {
    io_op_t op = {
      .type = IOTYPE__READ,
      .fildes = fildes,
      .buf = buf,
      .nbyte = nbyte,
      .res = 0,
      .fut = fut
    };
    io_queue_push(self->l->io_queue, &op);
  } else {
    io_future_result res;
    res.ret_val = syscall(SYS_read, fildes, buf, nbyte);
    res.errno_val = errno;
    ((base_io_fut*)fut.f)->put(std::move(res));
  }

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
      .res = 0,
      .fut = fut
    };

    io_queue_push(self->l->io_queue, &op);
  } else {
    io_future_result res;
    res.ret_val = syscall(SYS_write, fildes, buf, nbyte);
    res.errno_val = errno;
    ((base_io_fut*)fut.f)->put(std::move(res));
  }

  return fut;
}

io_future_result cilk_iosync(io_future *fut) {
  io_future_result ret = ((base_io_fut*)fut->f)->get();

  delete (base_io_fut*)(fut->f);
  fut->f = NULL;

  return ret;
}

}
