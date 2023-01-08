cc = g++-12  # note: in case of policies.cpp it is not possible to use clang.
ccflags = -Wall -Wextra -I ../ -std=c++2b
optflags = -fopenmp -ftree-vectorize -O2
sources = $(wildcard *.cpp)
targets = $(patsubst %.cpp,%.out,$(sources))

.PHONY: all

all: $(targets)

%.out: %.cpp
	$(cc) $(ccflags) $(optflags) -o  $@ $<
