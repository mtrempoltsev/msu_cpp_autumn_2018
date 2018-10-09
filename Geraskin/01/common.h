#pragma once

#include <cstddef>
#include <random>

namespace NParams {
    constexpr size_t NRows = 1e4;
    constexpr size_t NCols = 1e4;
    constexpr unsigned RandomSeed = 321;
}

using namespace NParams;

int** GetMatrix() {
    srand(RandomSeed);
    auto data = reinterpret_cast<int*>(operator new(sizeof(int) * NRows * NCols));
    auto matrix = reinterpret_cast<int**>(operator new(sizeof(int*) * NRows));

    for (size_t i = 0; i < NRows; ++i) {
        matrix[i] = data + i * NCols;
        for (size_t j = 0; j < NRows; ++j) {
            matrix[i][j] = rand();
        }
    }

    return matrix;
}

void DeleteMatrix(int** matrix) {
    delete[] matrix[0];
    delete[] matrix;
}
