#include <cilk/cilk.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "fib.h"
#include "fib-producer.h"
#include "fib-mr-options.h"

#include <inttypes.h>

#include <stdio.h>

int fib_n = 30;
int fib_count = 30000;
int io_delay = 50000;
int nruns = 1;

int getValue(int unused) {
  uint64_t in_buf;
  int prod_fd = create_producer(io_delay);
  read(prod_fd, &in_buf, sizeof(uint64_t));
  close(prod_fd);
  return fib_n;
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

int main(int argc, char *args[]) {
  load_fib_mr_options(argc, args);

  cilk_spawn run_bench(0, fib_count);
  cilk_sync;
  return 0;
}
