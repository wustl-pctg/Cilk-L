#include <cilk/cilk.h>
#include <cilk/cilk_io.h>

#include <string.h>

#include <unistd.h>

#include "fib.h"
#include "fib-producer.h"

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

int main(int argc, char *args[]) {
    int recv_fd = create_producer(5000);

    cilk_spawn run_bench(recv_fd);
    cilk_sync;

    //join_producer(&prod_args);
    close(recv_fd);

    return 0;
}
