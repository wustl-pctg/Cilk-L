#include <cilk/cilk.h>
#include <cilk/cilk_io.h>

#include <string.h>

#include <unistd.h>

#include "fib-consumer.h"
#include "fib-producer.h"

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

void wrap_fib(int n) {
    int res = fib(n);
    printf("Res: %d\n", res);
}

void run_bench(int fd) {
    uint64_t in_buf;
    volatile int val = 30;
    int res = 0;
    for (int i = 0; i < 3000; ) {
        res = read(fd, &in_buf, sizeof(uint64_t));

        for (uint64_t j = 0; j < in_buf && i < 3000; j++) {
            i++;
            cilk_spawn wrap_fib(val);
        }

    }
}

int main(void) {
    int recv_fd = create_producer(5000);

    cilk_spawn run_bench(recv_fd);
    cilk_sync;

    //join_producer(&prod_args);
    close(recv_fd);

    return 0;
}
