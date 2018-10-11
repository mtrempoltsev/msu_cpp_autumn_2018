#include <iostream>
#include "timer.h"

int main()
{
    const size_t arraySize = 1e5;
    int **matrix = new int*[arraySize];
    for(size_t i = 0; i < arraySize; ++i) {
        matrix[i] = new int[arraySize];
    }
    {
        size_t sum = 0;
        Timer timer;
        for (size_t i = 0; i < arraySize; ++i) {
            for (size_t j = 0; j < arraySize; ++j) {
                sum += matrix[i][j];
            }
        }
    }


    for(size_t i = 0; i < arraySize; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
