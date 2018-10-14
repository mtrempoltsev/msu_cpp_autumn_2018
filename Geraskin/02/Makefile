CC = g++
CPPFLAGS = -O2 -std=c++17
COMPILE = $(CC) $^ $(CPPFLAGS) -o $@

%PHONY: all clean

all : main

main : main.cpp
	$(COMPILE)

clean :
	rm -f main
