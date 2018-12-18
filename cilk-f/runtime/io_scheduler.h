#ifndef INCLUDE_IO_SCHEDULER_H
#define INCLUDE_IO_SCHEDULER_H

#ifdef __cplusplus
extern "C" {
#endif

void scheduler_thread_proc_for_io_worker(void* arg);

#ifdef __cplusplus
}
#endif

#endif
