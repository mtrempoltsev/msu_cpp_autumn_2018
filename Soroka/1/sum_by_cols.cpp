#include <iostream>
#include "timer.h"

int main()
{

    const int n = 1e4;
    int **matrix = new int*[n];
    for(size_t i = 0; i < n; ++i) {
        matrix[i] = new int[n];
    }

    Timer timer;
    int sum = 0;
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < n; ++i) {
            sum += matrix[i][j];
        }
    }

    for(size_t i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
