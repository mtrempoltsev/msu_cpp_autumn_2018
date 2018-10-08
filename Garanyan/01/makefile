CC=g++
CCFLAGS=--std=c++11 -O2

all: sum_by_rows sum_by_cols

sum_by_rows: sum_by_rows.cpp
	$(CC) $< -o $@ $(CCFLAGS)

sum_by_cols: sum_by_cols.cpp
	$(CC) $< -o $@ $(CCFLAGS)

clean:
	rm -rf *.o sum_by_rows sum_by_cols
