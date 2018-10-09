#include "common.h"
#include "timer.h"

using namespace NParams::NMatrix;

int main() {
    auto matrix = GetMatrix();

    {
        Timer timer;
        volatile int sum = 0;
        for (size_t i = 0; i < NRows; ++i) {
            for (size_t j = 0; j < NCols; ++j) {
                sum += matrix[i][j];
            }
        }
    }

    DeleteMatrix(matrix);
}
