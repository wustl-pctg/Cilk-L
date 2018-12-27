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

getoptions.o: getoptions.cpp
	$(CXX) $(FUTURE_CXXFLAGS) -c getoptions.cpp -o getoptions.o

fib.o: fib.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib.c -o fib.o

fib-producer.o: fib-producer.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-producer.c -o fib-producer.o

fib-options.o: fib-options.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-options.c -o fib-options.o

TARGETS = fib-srv-l
APPS = fib-srv-l


fib-srv-l: clean fib-srv-l.c fib.o fib-producer.o getoptions.o fib-options.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-srv-l.c -o fib-srv-l.o
	$(CXX) -flto fib-srv-l.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-srv-l $(FUTURE_LDFLAGS)

TARGETS += fib-srv-fj
APPS += fib-srv-fj

fib-srv-fj: clean fib-srv-fj.c fib.o fib-producer.o getoptions.o fib-options.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-srv-fj.c -o fib-srv-fj.o
	$(CXX) -flto fib-srv-fj.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-srv-fj $(FUTURE_LDFLAGS)

TARGETS += fib-mr-l
APPS += fib-mr-l

fib-mr-l: clean fib-mr-l.c fib.o fib-producer.o getoptions.o fib-options.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-mr-l.c -o fib-mr-l.o
	$(CXX) -flto fib-mr-l.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-mr-l $(FUTURE_LDFLAGS)

TARGETS += fib-mr-fj
APPS += fib-mr-fj

fib-mr-fj: clean fib-mr-fj.c fib.o fib-producer.o getoptions.o fib-options.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-mr-fj.c -o fib-mr-fj.o
	$(CXX) -flto fib-mr-fj.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-mr-fj $(FUTURE_LDFLAGS)


###########################################################################
# Though shalt not cross this line lest thou knowest what thou art doing! #
###########################################################################

all: $(TARGETS)

clean:
	rm -f *.o $(APPS)
