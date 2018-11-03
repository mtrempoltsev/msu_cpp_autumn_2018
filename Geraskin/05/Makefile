CC = g++
CPPFLAGS = -O2 -std=c++17
COMPILE = $(CC) $^ $(CPPFLAGS) -o $@

%PHONY: all clean

all : test

test : test.cpp
	$(COMPILE)

clean :
	rm -f test
