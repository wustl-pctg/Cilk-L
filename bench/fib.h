#ifndef __INCLUDED__FIB__H__
#define __INCLUDED__FIB__H__

extern int serial_base_case;

// Serial recursive fibonacci calculation
int serial_fib(int n);

// Fork-join recursive fibonacci calculation
int fib(int n);

// Run the fib function and print the result
int wrap_fib(int n);

#endif
