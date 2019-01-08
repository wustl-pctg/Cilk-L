#include <cilk/cilk.h>

#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "fib-producer.h"

#include <inttypes.h>

#include <stdio.h>

int serial_base_case = 15;

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
