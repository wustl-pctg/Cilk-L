#ifndef INCLUDED_CILK_IO_H
#define INCLUDED_CILK_IO_H

#include <sys/types.h>

#ifdef __cplusplus

#include <cilk/future.h>

extern "C" {

#endif

typedef struct io_future_result {
  ssize_t ret_val;
  int     errno_val;
} io_future_result;

typedef struct io_future {
  void *f; // A pointer to the underlying future type (c++ object)
} io_future;

extern io_future cilk_read(int fildes, void *buf, size_t nbyte);
extern io_future cilk_write(int fildes, void *buf, size_t nbyte);
extern io_future_result cilk_iosync(io_future *fut);

#ifdef __cplusplus

typedef cilk::future<io_future_result> base_io_fut;

}
#endif

#endif
