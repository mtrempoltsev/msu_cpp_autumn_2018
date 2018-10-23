CC = g++
CPPFLAGS = -O2 -std=c++17
COMPILE = $(CC) $^ $(CPPFLAGS) -o $@

%PHONY: all clean

all : calc

calc : calc.cpp
	$(COMPILE)

clean :
	rm -f calc
