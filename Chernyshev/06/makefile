CPPC = g++
CPPFLAGS = -O2 -std=c++14

%PHONY: all clean

all: test

test: test.cpp serializer.h
	$(CPPC) $(CPPFLAGS) $< -o $@

clean:
	rm -f test
