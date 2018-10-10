#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us =
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};

const size_t maxRows = 10 * 1024;
const size_t maxColumns = 10 * 1024;

using Type = uint32_t;
using MatrixType = Type(*)[maxColumns];

void generate(MatrixType matrix)
{
    for (size_t y = 0; y < maxRows; ++y)
    {
        for (size_t x = 0; x < maxColumns; ++x)
        {
            matrix[y][x] = x + y;
        }
    }
}

size_t sumByRow(MatrixType matrix)
{
    size_t s = 0;
    for (size_t y = 0; y < maxRows; ++y)
    {
        s += y;
        for (size_t x = 0; x < maxColumns; ++x)
        {
            s += matrix[y][x];
        }
    }
    return s;
}

size_t sumByColumn(MatrixType matrix)
{
    size_t s = 0;
    for (size_t x = 0; x < maxColumns; ++x)
    {
        s += x;
        for (size_t y = 0; y < maxRows; ++y)
        {
            s += matrix[y][x];
        }
    }
    return s;
}

int main()
{
    auto blob = std::make_unique<Type[]>(maxRows * maxColumns);
    auto matrix = reinterpret_cast<MatrixType>(blob.get());

    generate(matrix);

    const size_t repeats = 3;

    size_t s = 0;

    for (size_t i = 0; i < repeats; ++i)
    {
        Timer timer;
        s += sumByColumn(matrix);
    }

    for (size_t i = 0; i < repeats; ++i)
    {
        Timer timer;
        s += sumByRow(matrix);
    }

    return s;
}
