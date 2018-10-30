#include <stdexcept>
#include <cstring>
#include "matrix.h"

Matrix::Matrix(size_t rows, size_t cols)
    : rows(rows)
    , cols(cols)
{
    data = new int[cols * rows];
}

Matrix::~Matrix()
{
    delete[] data;
}

Matrix::Row::Row(int * data, size_t cols)
    : data(data)
    , cols(cols)
{
}

Matrix::Row Matrix::operator[](size_t row)
{
    if (row >= rows)
        throw std::out_of_range("");
    return Row(&data[row * cols], cols);
}

const Matrix::Row Matrix::operator[](size_t row) const
{
    if (row >= rows)
        throw std::out_of_range("");
    return Row(&data[row * cols], cols);
}

int & Matrix::Row::operator[](size_t col)
{
    if (col >= cols)
        throw std::out_of_range("");
    return data[col];
}

const int & Matrix::Row::operator[](size_t col) const
{
    if (col >= cols)
        throw std::out_of_range("");
    return data[col];
}

bool Matrix::operator==(const Matrix & other) const
{
    if (rows == other.rows && cols == other.cols) {
        return !memcmp(data, other.data, cols * rows * sizeof(int));
    }
    return false;
}

bool Matrix::operator!=(const Matrix & other) const
{
    return !(*this == other);
}

Matrix & Matrix::operator*=(int value)
{
    for(size_t i = 0; i < rows * cols; ++i) {
        data[i] *= value;
    }
    return *this;
}

size_t Matrix::getColumns() const
{
    return cols;
}

size_t Matrix::getRows() const
{
    return rows;
}
