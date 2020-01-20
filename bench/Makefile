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
COMMON_LDFLAGS  = ktiming.o -L$(mkfile_dir)/../SuperMalloc/release/lib -lsupermalloc -lrt -ldl -lpthread -lnuma -lpthread -lm -lrt

FUTURE_CXXFLAGS = $(COMMON_CXXFLAGS) -I../cilk-l/runtime/config/x86 -I../cilk-l/runtime/sslib -I../cilk-l/include
FUTURE_LDFLAGS 	= $(CILKF_A) $(COMMON_LDFLAGS) 

VANILLA_CXXFLAGS = $(COMMON_CXXFLAGS) -I../cilk-l/runtime/config/x86 -I../cilk-l/runtime/sslib -I../cilk-l/include
VANILLA_LDFLAGS  = $(CILKPLUS_A) $(COMMON_LDFLAGS) 

.PHONY: default all clean
default: all

getoptions.o: getoptions.cpp
	$(CXX) $(FUTURE_CXXFLAGS) -c getoptions.cpp -o getoptions.o

ktiming.o: ktiming.c
	$(CXX) $(FUTURE_CXXFLAGS) -c ktiming.c -o ktiming.o

fib.o: fib.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib.c -o fib.o

fib-producer.o: fib-producer.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-producer.c -o fib-producer.o

fib-options.o: fib-options.c
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-options.c -o fib-options.o

TARGETS = fib-srv-se
APPS = fib-srv-se

fib-srv-se: fib-srv-se.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(COMMON_CXXFLAGS) -c fib-srv-se.c -o fib-srv-se.o
	$(CXX) -flto fib-srv-se.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-srv-se $(COMMON_LDFLAGS)

TARGETS = fib-srv-l
APPS = fib-srv-l

fib-srv-l: $(CILKF_A) fib-srv-l.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-srv-l.c -o fib-srv-l.o
	$(CXX) -flto fib-srv-l.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-srv-l $(FUTURE_LDFLAGS)

TARGETS += fib-srv-fj
APPS += fib-srv-fj

fib-srv-fj: $(CILKPLUS_A) fib-srv-fj.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-srv-fj.c -o fib-srv-fj.o
	$(CXX) -flto fib-srv-fj.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-srv-fj $(FUTURE_LDFLAGS)

TARGETS += fib-sio-se
APPS += fib-sio-se

fib-sio-se: fib-sio-se.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(COMMON_CXXFLAGS) -c fib-sio-se.c -o fib-sio-se.o
	$(CXX) -flto fib-sio-se.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-sio-se $(COMMON_LDFLAGS)

TARGETS += fib-sio-l
APPS += fib-sio-l

fib-sio-l: $(CILKF_A) fib-sio-l.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-sio-l.c -o fib-sio-l.o
	$(CXX) -flto fib-sio-l.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-sio-l $(FUTURE_LDFLAGS)

TARGETS += fib-sio-fj
APPS += fib-sio-fj

fib-sio-fj: $(CILKPLUS_A) fib-sio-fj.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-sio-fj.c -o fib-sio-fj.o
	$(CXX) -flto fib-sio-fj.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-sio-fj $(FUTURE_LDFLAGS)

TARGETS += fib-mr-se
APPS += fib-mr-se

fib-mr-se: fib-mr-se.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(COMMON_CXXFLAGS) -c fib-mr-se.c -o fib-mr-se.o
	$(CXX) -flto fib-mr-se.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-mr-se $(COMMON_LDFLAGS)

TARGETS += fib-mr-l
APPS += fib-mr-l

fib-mr-l: $(CILKF_A) fib-mr-l.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-mr-l.c -o fib-mr-l.o
	$(CXX) -flto fib-mr-l.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-mr-l $(FUTURE_LDFLAGS)

TARGETS += fib-mr-fj
APPS += fib-mr-fj

fib-mr-fj: $(CILKPLUS_A) fib-mr-fj.c fib.o fib-producer.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-mr-fj.c -o fib-mr-fj.o
	$(CXX) -flto fib-mr-fj.o fib.o fib-producer.o getoptions.o fib-options.o -o fib-mr-fj $(FUTURE_LDFLAGS)

TARGETS += fib-bench-se
APPS += fib-bench-se

fib-bench-se: $(CILKPLUS_A) fib-bench-se.c fib.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(COMMON_CXXFLAGS) -c fib-bench-se.c -o fib-bench-se.o
	$(CXX) -flto fib-bench-se.o fib.o fib-options.o getoptions.o -o fib-bench-se $(COMMON_LDFLAGS)

TARGETS += fib-bench
APPS += fib-bench

fib-bench: fib-bench.c fib.o getoptions.o fib-options.o ktiming.o
	$(CXX) $(FUTURE_CXXFLAGS) -c fib-bench.c -o fib-bench.o
	$(CXX) -flto fib-bench.o fib.o fib-options.o getoptions.o -o fib-bench $(FUTURE_LDFLAGS)

###########################################################################
# Though shalt not cross this line lest thou knowest what thou art doing! #
###########################################################################

all: $(TARGETS)

clean:
	rm -f *.o $(APPS)
