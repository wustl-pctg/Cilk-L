#include <cilk/cilk.h>
#include <cilk/cilk_io.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>

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
#include <assert.h>
void run_bench(int fd) {
    io_future_result io_res = { 0, 0};
    io_future fut;
    uint64_t in_buf;
    volatile int val = 30;
    //do {
    for (int i = 0; i < 3000; ) {
        fut = cilk_read(fd, &in_buf, sizeof(uint64_t));
        io_res = cilk_iosync(&fut);

        //if (io_res.ret_val <= 0) {
        //  assert(in_buf[0] == 'q');
          //fprintf(stderr, "ERROR READING INPUT: %s\n", strerror(io_res.errno_val));
          //exit(1);
        //} else {
        //    in_buf[io_res.ret_val] = 0;
        //}

        //val = atoi(in_buf);
        //if (val != 0) {
        for (uint64_t j = 0; j < in_buf && i < 3000; j++) {
          i++;
          cilk_spawn wrap_fib(val);
        }
        //}

        //assert(io_res.ret_val != 0);
    }
    //} while(in_buf[0] != 'q');
}

int main(void) {
    int recv_fd = create_producer(5000);

    //int recv_fd = open_consumer();
    int saved_flags = fcntl(recv_fd, F_GETFL);
    fcntl(recv_fd, F_SETFL, saved_flags | O_NONBLOCK);

    //int recvd = 0;

    //char tmp[512];

    /*do {
        recvd = read(recv_fd, tmp, 511);
        if (recvd == -1) {
            printf("Would have blocked!\n");
        }
    } while (recvd == -1);

    tmp[recvd] = 0;
    printf("Got %s\n", tmp);
    */

    cilk_spawn run_bench(recv_fd);
    cilk_sync;

    //join_producer(&prod_args);
    close(recv_fd);

    return 0;
}
