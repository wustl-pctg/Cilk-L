#include <cilk/cilk_io.h>
#include <cilk/future.h>
#include <unistd.h>

extern "C" {

io_future cilk_read(int fildes, void *buf, size_t nbyte) {
  cilk::future<ssize_t> *fut = new cilk::future<ssize_t>();

  fut->put(read(fildes, buf, nbyte));

  return (void*)fut;
}

io_future cilk_write(int fildes, void *buf, size_t nbyte) {
  cilk::future<ssize_t> *fut = new cilk::future<ssize_t>();

  fut->put(write(fildes, buf, nbyte));

  return (void*)fut;
}

ssize_t cilk_iosync(io_future *fut) {
  ssize_t ret = ((cilk::future<ssize_t> *)*fut)->get();

  delete (cilk::future<ssize_t>*)(*fut);
  *fut = NULL;

  return ret;
}

}
