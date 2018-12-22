#include <cilk/cilk.h>
#include <cilk/cilk_io.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "fib-producer.h"

#include <inttypes.h>

#include <stdio.h>

static int serial_base_case = 2;

int serial_fib(int n) {
    if (n < 2) {
      return n;
    }

    int x = serial_fib(n-1);
    int y = serial_fib(n-2);
    return x+y;
}

int fib(int n) {
    if (n < serial_base_case) {
        return serial_fib(n);
    }

    int x = 0;
    int y = 0;
    
    x = cilk_spawn fib(n-1);
    y = fib(n-2);
    cilk_sync;
    return x+y;
}

int wrap_fib(int n) {
    int res = fib(n);
    printf("Res: %d\n", res);
    return res;
}

int getValue(int unused) {
  uint64_t in_buf;
  int prod_fd = create_producer(50000);
  int saved_flags = fcntl(prod_fd, F_GETFL);
  fcntl(prod_fd, F_SETFL, saved_flags | O_NONBLOCK);
  io_future f = cilk_read(prod_fd, &in_buf, sizeof(uint64_t));
  cilk_iosync(&f);
  close(prod_fd);
  return 30;
}

int run_bench(int start, int end) {
  int n = end - start;
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    int x = getValue(start);
    return wrap_fib(x);
  } else {
    int piv = (start + end) / 2;
    int res1 = 0;
    res1 = cilk_spawn run_bench(start, piv);
    int res2 = run_bench(piv, end);
    cilk_sync;
    return res1 + res2;
  }
}

int main(void) {
  cilk_spawn run_bench(0, 30000);
  cilk_sync;
  return 0;
}
