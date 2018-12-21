#include <cilk/cilk.h>
#include <cilk/cilk_io.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>

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
    char in_buf[3];
    int val = 0;
    int res = 0;
    do {
        res = read(fd, in_buf, 3);

        if (res <= 0) {
          fprintf(stderr, "ERROR READING INPUT: %s\n", strerror(errno));
          exit(1);
        }

        in_buf[res-1] = 0;
        val = atoi(in_buf);
        if (val != 0) {
          cilk_spawn wrap_fib(val);
        }

    } while(strcmp(in_buf, "q") != 0);
}

int main(void) {
    cilk_spawn run_bench(STDIN_FILENO);
    cilk_sync;

    return 0;
}
