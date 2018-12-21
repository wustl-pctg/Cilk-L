###########################################################################
# NOTE: Thou shalt add thine build targets to the TARGETS designator, and #
#       thou shalt add thine resulting executables to the APPS designator,#
#       lest thou desire to be charged with polluting the systems of      #
#       thine fellows.                                                    #
###########################################################################

# This MUST be before any includes
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(patsubst %/,%,$(dir $(mkfile_path)))

include ../common.mk

COMMON_CXXFLAGS = -DTIMES_TO_RUN=10 -DTIMING_COUNT=10 -O3 -flto -g -fcilkplus -fno-omit-frame-pointer -fcilk-no-inline -I../src -I../llvm-cilk/include -std=c++11 -Wall
COMMON_LDFLAGS  = -L$(mkfile_dir)/../SuperMalloc/release/lib -lsupermalloc -lrt -ldl -lpthread -lnuma -lpthread -lm -lrt

FUTURE_CXXFLAGS = $(COMMON_CXXFLAGS) -I../cilk-f/runtime/config/x86 -I../cilk-f/runtime/sslib -I../cilk-f/include
FUTURE_LDFLAGS 	= ../cilk-f/.libs/libcilkrts.a $(COMMON_LDFLAGS) 

VANILLA_CXXFLAGS = $(COMMON_CXXFLAGS) -I../cilk-f/runtime/config/x86 -I../cilk-f/runtime/sslib -I../cilk-f/include
VANILLA_LDFLAGS  = ../cilk-f/.libs/libcilkrts.a $(COMMON_LDFLAGS) 

.PHONY: default all clean
default: all

TARGETS = fib-l
APPS = fib-l

fib-l: clean fib-l.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-l.c -o fib-l.o
	$(CXX) -flto fib-l.o -o fib-l $(FUTURE_LDFLAGS)

TARGETS += fib-fj
APPS = fib-fj

fib-fj: clean fib-fj.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-fj.c -o fib-fj.o
	$(CXX) -flto fib-fj.o -o fib-fj $(FUTURE_LDFLAGS)


###########################################################################
# Though shalt not cross this line lest thou knowest what thou art doing! #
###########################################################################

all: $(TARGETS)

clean:
	rm -f *.o $(APPS)
