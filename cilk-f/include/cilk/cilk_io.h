#ifndef INCLUDED_CILK_IO_H
#define INCLUDED_CILK_IO_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* io_future;

extern io_future cilk_read(int fildes, void *buf, size_t nbyte);
extern io_future cilk_write(int fildes, void *buf, size_t nbyte);
extern ssize_t cilk_iosync(io_future *fut);

#ifdef __cplusplus
}
#endif

#endif
