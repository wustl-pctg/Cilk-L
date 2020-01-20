In this version of Cilk-L, the default configuration is that I/O threads and work threads are bound to the same logical CPU core.
In the corresponding paper, the configuration is such that I/O threads and work threads are bound to the same physical CPU, but
such that the work thread is on one hyperthread (logical CPU on the same physical core) and the I/O thread is on the other hyperthread.
The configuration can be changed by modifying the g-\>io\_mode variable around line 430 of cilk-l/runtime/global\_state.cpp

It is also important to note that because the Cilk ABI has been modified to support futures, the entirety of this repo must be built
using the provided custom clang implementation that, when given the -fcilk-no-inline, will not inline ABI functions and will instead
call the functions linked in.
