cc = clang++  # note: in case of policies.cpp it is not possible to use clang.
ccflags = -Wall -I ../ -std=c++20 -O2
sources = $(wildcard *.cpp)
targets = $(patsubst %.cpp,%.out,$(sources))

.PHONY: all

all: $(targets)

%.out: %.cpp
	$(cc) $(ccflags) -o  $@ $<
