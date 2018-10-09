#include <chrono>
#include <iostream>
#include "timer.h"


int main(){
    
    size_t rows = 10000;
    size_t cols = 10000;
    
    int **matrix = new int*[rows];
    
    for(size_t i = 0; i<rows; i++)
        matrix[i] = new int[cols];
    
    for(size_t i = 0; i<rows; i++)
        for(size_t j = 0; j<cols; j++)
            matrix[i][j] = i+j;
    
    {
        Timer timer;
        volatile int sum = 0;
        for(size_t i = 0; i<rows; i++)
            for(size_t j = 0; j<cols; j++)
                sum += matrix[i][j];
    }
    
    for(size_t i = 0; i<rows; i++)
        delete []matrix[i];
    
    delete []matrix;
    
    return 0;
}
