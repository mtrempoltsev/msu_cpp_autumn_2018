#pragma once
#include <cstddef>

class Matrix
{
public:
    class Row
    {
    public:
        Row(int * data, size_t cols);

        int & operator[](size_t col);
        const int & operator[](size_t col) const;
    private:
        int * data;
        size_t cols;
    };

    Matrix(size_t rows, size_t cols);
    ~Matrix();

    Row operator[](size_t row);
    const Row operator[](size_t row) const;
    bool operator==(const Matrix & other) const;
    bool operator!=(const Matrix & other) const;
    Matrix & operator*=(int value);

    size_t getColumns() const;
    size_t getRows() const;

private:
    int * data;
    size_t cols;
    size_t rows;
};
