#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/timerfd.h>

#include <arpa/inet.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "fib-producer.h"

int create_producer(int usec_timeout) {
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);

    struct itimerspec timeout;
    timeout.it_interval = {
        .tv_sec = usec_timeout / 1000000,
        .tv_nsec = (usec_timeout % 1000000) * 1000
    }; 

    timeout.it_value = timeout.it_interval;

    timerfd_settime(timer_fd, 0, &timeout, NULL);

    return timer_fd;
}

void join_producer(producer_args_t *args) {
}
