#pragma once
#include <new>
#include <stdexcept>

class Matrix
{
        int rows_n, cols_n, elms_n;
        class Row
        {
                int* offt;
                int cols;
            public:
                explicit Row(int* oofft, int ccols): offt{oofft},
                    cols{ccols} {};
                int& operator[](int index)
                {
                    if (index < 0 || index >= cols) {
                        throw std::out_of_range("");
                    } else {
                        return offt[index];
                    }
                }
                const int& operator[](int index) const
                {
                    if (index < 0 || index >= cols) {
                        throw std::out_of_range("");
                    } else {
                        return offt[index];
                    }
                }
        };
    public:
        explicit Matrix(int nrows, int ncols):
            rows_n{nrows}, cols_n{ncols}, elms_n{nrows * ncols},
            m{static_cast<int*>(::operator new[] (sizeof(int) * elms_n))}
        {
            for (int i = 0; i < elms_n; ++i) {
                new (m + i) int{0};
            }
        }
        ~Matrix()
        {
            ::operator delete[] (m, sizeof(int) * elms_n);
        }
        const Row operator[](int row) const;
        Row operator[](int row);
        bool operator==(const Matrix& that) const;
        bool operator!=(const Matrix& that) const;
        Matrix& operator*=(int);
        int getRows() const;
        int getColumns() const;
        int* m;
};

int Matrix::getRows() const
{
    return rows_n;
}

int Matrix::getColumns() const
{
    return cols_n;
}

const Matrix::Row Matrix::operator[](int row) const
{
    if (row < 0 || row >= rows_n) {
        throw std::out_of_range("");
    } else {
        return Matrix::Row{m + cols_n * row, cols_n};
    }
}

Matrix::Row Matrix::operator[](int row)
{
    if (row < 0 || row >= rows_n) {
        throw std::out_of_range("");
    } else {
        return Matrix::Row{m + cols_n * row, cols_n};
    }
}

bool Matrix::operator==(const Matrix& that) const
{
    if (rows_n != that.rows_n || cols_n != that.cols_n) {
        return false;
    } else {
        for (int i = 0; i < elms_n; ++i) {
            if (m[i] != that.m[i]) {
                return false;
            }
        }
        return true;
    }
}

bool Matrix::operator!=(const Matrix& that) const
{
    return !(Matrix::operator==(that));
}

Matrix& Matrix::operator*=(int mult)
{
    for (int i = 0; i < elms_n; ++i) {
        m[i] *= mult;
    }
    return *this;
}
