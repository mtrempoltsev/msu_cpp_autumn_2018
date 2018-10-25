#pragma once
#include <new>
#include <stdexcept>

class Matrix
{
        int rows_n, cols_n, elms_n;
        int* m;
        class Row
        {
                int* offt;
                int cols;
            public:
                explicit Row(int* oofft, int ccols): offt{oofft},
                    cols{ccols} {};
                int& operator[](int index)
                {
                    if (index < 0 || index > cols) {
                        throw std::out_of_range("");
                    } else {
                        return offt[index];
                    }
                }
                const int& operator[](int index) const
                {
                    if (index < 0 || index > cols) {
                        throw std::out_of_range("");
                    } else {
                        return offt[index];
                    }
                }
        };
    public:
        explicit Matrix(int nrows, int ncols):
            rows_n{nrows}, cols_n{ncols}, elms_n{nrows * ncols},
            m{static_cast<int*>(::operator new[] (sizeof(int) * nrows * ncols))}
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
        Matrix operator*=(int);
        const int* get_rawaddr() const;
        int* get_rawaddr();
        int get_rows() const;
        int get_cols() const;
};

int Matrix::get_rows() const
{
    return rows_n;
}

int Matrix::get_cols() const
{
    return cols_n;
}

const Matrix::Row Matrix::operator[](int row) const
{
    if (row < 0 || row > rows_n) {
        throw std::out_of_range("");
    } else {
        return Matrix::Row{m + cols_n * row, cols_n};
    }
}

Matrix::Row Matrix::operator[](int row)
{
    if (row < 0 || row > rows_n) {
        throw std::out_of_range("");
    } else {
        return Matrix::Row{m + cols_n * row, cols_n};
    }
}

const int* Matrix::get_rawaddr() const
{
    return const_cast<const int*>(m);
}

int* Matrix::get_rawaddr()
{
    return m;
}

bool Matrix::operator==(const Matrix& that) const
{
    if (rows_n != that.rows_n || cols_n != that.cols_n) {
        return false;
    } else {
        for (int i = 0; i < elms_n; ++i) {
            if (m[i] != that.get_rawaddr()[i]) {
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

Matrix Matrix::operator*=(int mult)
{
    for (int i = 0; i < elms_n; ++i) {
        m[i] *= mult;
    }
    return *this;
}
