CC = g++
CCFLAGS = --std=c++11 -O2

%PHONY: all clean

all : test

test : test.cpp
	$(CC) $^ -o $@ $(CCFLAGS)

clean :
	rm -f test
