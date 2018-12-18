#ifndef INCLUDED_CILK_IO_H
#define INCLUDED_CILK_IO_H

#include <sys/types.h>

#ifdef __cplusplus

#include <cilk/future.h>
typedef cilk::future<ssize_t> base_io_fut;

extern "C" {

#endif

typedef struct io_future {
  void *f; // A pointer to the underlying future type (c++ object)
} io_future;

extern io_future cilk_read(int fildes, void *buf, size_t nbyte);
extern io_future cilk_write(int fildes, void *buf, size_t nbyte);
extern ssize_t cilk_iosync(io_future *fut);

#ifdef __cplusplus
}
#endif

#endif
