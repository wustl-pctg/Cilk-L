BASE_DIR=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(BASE_DIR)/config.mk

CC=$(COMPILER_HOME)/bin/clang
CXX=$(COMPILER_HOME)/bin/clang++

INC = -I$(RUNTIME_HOME)/include
LDFLAGS = -ldl -lpthread -L$(BASE_DIR)/SuperMalloc/release/lib -lsupermalloc
ARFLAGS = rcs
OPT = -O3 #-march=native -DNDEBUG

CILKF_A = $(BASE_DIR)/cilk-f/.libs/libcilkrts.a
CILKPLUS_A = $(BASE_DIR)/cilk-f/.libs/libcilkrts.a

CFLAGS = -g -Wfatal-errors $(INC)
CILKFLAGS = -fcilkplus -fcilk-no-inline
