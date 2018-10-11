CC = g++
CPPFLAGS = -O2 -std=c++17
COMPILE = $(CC) $^ $(CPPFLAGS) -o $@

%PHONY: all clean

all : sum_by_rows sum_by_cols

sum_by_rows : sum_by_rows.cpp
	$(COMPILE)

sum_by_cols : sum_by_cols.cpp
	$(COMPILE)

clean :
	rm -f sum_by_rows sum_by_cols
