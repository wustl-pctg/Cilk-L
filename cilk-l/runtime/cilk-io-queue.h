#ifndef INCLUDED_CILK_IO_QUEUE_H
#define INCLUDED_CILK_IO_QUEUE_H

#include <sys/types.h>
#include <cilk/cilk_io.h>

typedef struct io_op_t {
  int type;
  int fildes;
  void *buf;
  size_t nbyte;
  ssize_t res;
  io_future fut;
} io_op_t;

#define IOTYPE__INVALID   (-1)
#define IOTYPE__READ      (0)
#define IOTYPE__WRITE     (1)

#define DEFAULT_IO_Q_SIZE (4096)
#define IO_Q_SIZE(q)      (DEFAULT_IO_Q_SIZE)

typedef struct io_queue_t {
  volatile size_t head;
  volatile size_t tail;
  int eventfd;
  volatile io_op_t q[DEFAULT_IO_Q_SIZE];
} io_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

io_queue_t* new_io_queue();
void del_io_queue(io_queue_t* q);

void io_queue_push(io_queue_t* q, io_op_t* op);
io_op_t io_queue_pop(io_queue_t* q);

#ifdef __cplusplus
}
#endif

#endif
