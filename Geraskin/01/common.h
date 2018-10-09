#pragma once

#include <cstddef>
#include <random>

namespace NParams {
    namespace NMatrix {
        constexpr size_t NRows = 1e4;
        constexpr size_t NCols = 1e4;
    }

    namespace NRandom {
        constexpr unsigned Seed = 321;
        constexpr int Max = 1e3;
        constexpr int Min = -1e3;
    }
}

int** GetMatrix() {
    using namespace NParams::NRandom;
    using namespace NParams::NMatrix;

    srand(Seed);
    auto data = reinterpret_cast<int*>(operator new(sizeof(int) * NRows * NCols));
    auto matrix = reinterpret_cast<int**>(operator new(sizeof(int*) * NRows));

    std::default_random_engine generator(Seed);
    std::uniform_int_distribution uniform(Min, Max);

    for (size_t i = 0; i < NRows; ++i) {
        matrix[i] = data + i * NCols;
        for (size_t j = 0; j < NCols; ++j) {
            matrix[i][j] = uniform(generator);
        }
    }

    return matrix;
}

void DeleteMatrix(int** matrix) {
    delete[] matrix[0];
    delete[] matrix;
}
