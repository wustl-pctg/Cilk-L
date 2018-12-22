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

/*static pthread_t prod_thread;

void* producer_thread_func(void *args) {
    producer_args_t *opts = (producer_args_t*)args;

    //int sock_fd;
    int comm_fd;
    struct sockaddr_storage client_addr;

    int err = listen(opts->sock_fd, 1);
    if (err == -1) {
        printf("ERR: error in listen() (%s)\n", strerror(errno));
        exit(1);
    }

    socklen_t addr_size = sizeof(client_addr);
    comm_fd = accept(opts->sock_fd, (struct sockaddr *)&client_addr, &addr_size);
    close(opts->sock_fd);

    if (comm_fd == -1) {
        printf("ERR: error in accept() (%s)\n", strerror(errno));
        exit(1);
    }

    write(comm_fd, "start", strlen("start"));

    for (int i = 0; i < opts->num_writes; i++) {
        usleep(opts->sleep_usec);
        write(comm_fd, opts->fib_number, strlen(opts->fib_number));
    }

    write(comm_fd, "q", 1);
    //usleep(opts->sleep_usec);
    opts->sock_fd = comm_fd;
    close(comm_fd);

    return NULL;
}
*/
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
    /*struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(9001); 

    args->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (args->sock_fd == -1) {
        printf("ERR: could not create server socket (%s)\n", strerror(errno));
        exit(1);
    }

    int err = bind(args->sock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    if (err == -1) {
        printf("ERR: could not bind server socket (%s)\n", strerror(errno));
        exit(1);
    }

    err = pthread_create(&prod_thread, NULL, producer_thread_func, args);
    if (err == -1) {
        printf("ERR: could not create producer thread (%s)\n", strerror(errno));
        exit(1);
    }
    */
}

void join_producer(producer_args_t *args) {
    //pthread_join(prod_thread, NULL);
    //close(args->sock_fd);
}
