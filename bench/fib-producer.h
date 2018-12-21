#ifndef __INCLUDE_FIB_PRODUCER__H__
#define __INCLUDE_FIB_PRODUCER__H__

typedef struct producer_args_t {
    int sleep_usec;
    int num_writes;
    int fib_number;
    int sock_fd;
} producer_args_t;

void create_producer(producer_args_t *args);
void join_producer(producer_args_t *args);

//void producer_thread_func(void *args);

#endif
