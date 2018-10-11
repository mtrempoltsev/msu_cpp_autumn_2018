#!/bin/bash

make --silent clean
make --silent

echo "sum_by_rows"
./sum_by_rows

echo "sum_by_cols"
./sum_by_cols

make --silent clean
