all: sum_by_rows sum_by_columns

sum_by_rows: sum_by_rows.cpp
	g++ sum_by_rows.cpp -o sum_by_rows --std=c++14 -O2

sum_by_columns: sum_by_columns.cpp
	g++ sum_by_columns.cpp -o sum_by_columns --std=c++11 -O2

clear:
	rm -rf sum_by_rows sum_by_columns
