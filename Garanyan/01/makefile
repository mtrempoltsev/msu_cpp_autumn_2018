CC=g++
CCFLAGS=--std=c++11 -O2

all: test

test: test.cpp
	$(CC) $< -o $@ $(CCFLAGS)

clean:
	rm -rf *.o test
