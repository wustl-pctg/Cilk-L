#include <cilk/cilk.h>
#include <cilk/cilk_io.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "fib.h"
#include "fib-producer.h"
#include "fib-options.h"

int fib_n = 30;
int fib_count = 3000;
int io_delay = 50000;
int nruns = 1;

void run_bench(int fd) {
    io_future_result io_res = { 0, 0};
    io_future fut;
    uint64_t in_buf;

    for (int i = 0; i < fib_count; ) {
        fut = cilk_read(fd, &in_buf, sizeof(uint64_t));
        io_res = cilk_iosync(&fut);

        for (uint64_t j = 0; j < in_buf && i < fib_count; j++) {
          i++;
          cilk_spawn wrap_fib(fib_n);
        }
    }
}

int main(int argc, char *args[]) {
    load_fib_options(argc, args);

    int recv_fd = create_producer(io_delay);

    int saved_flags = fcntl(recv_fd, F_GETFL);
    fcntl(recv_fd, F_SETFL, saved_flags | O_NONBLOCK);

    cilk_spawn run_bench(recv_fd);
    cilk_sync;

    close(recv_fd);

    return 0;
}
