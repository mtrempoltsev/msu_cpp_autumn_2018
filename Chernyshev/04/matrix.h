#pragma once

class Matrix
{
private:
    size_t rows = 0, cols = 0;
    int *data = nullptr;

    class Proxy
    {
    public:
        Proxy(int *off_, size_t cols_)
                : off(off_), cols(cols_)
        {
        }

        int &operator[] (size_t col)
        {
            if (col >= cols) {
                throw std::out_of_range("col number should be between 0 and number of cols");
            }
            return off[col];
        }

        int operator[] (size_t col) const
        {
            if (col >= cols) {
                throw std::out_of_range("col number should be between 0 and number of cols");
            }
            return off[col];
        }

    private:
        int *off;
        size_t cols;
    };

public:
    Matrix(size_t rows_, size_t cols_)
            : rows(rows_), cols(cols_)
    {
        if (rows > 0 && cols > 0) {
            data = new int[rows * cols]();
        }
    }

    auto getRows() const
    {
        return rows;
    }

    auto getColumns() const
    {
        return cols;
    }

    Proxy operator [] (size_t row)
    {
        if (row >= rows) {
            throw std::out_of_range("row number should be between 0 and number of rows");
        }
        return Proxy(data + row * cols, cols);
    }

    const Proxy operator [] (size_t row) const
    {
        if (row >= rows) {
            throw std::out_of_range("row number should be between 0 and number of rows");
        }
        return Proxy(data + row * cols, cols);
    }

    Matrix &operator *= (int k)
    {
        for (size_t i = 0; i < rows * cols; ++i) {
            data[i] *= k;
        }
        return *this;
    }

    bool operator == (const Matrix &m) const
    {
        if (rows != m.rows || cols != m.cols) {
            return false;
        }

        for (size_t i = 0; i < rows * cols; ++i) {
            if (data[i] != m.data[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator != (const Matrix &m) const
    {
        return !(*this == m);
    }

    ~Matrix()
    {
        delete[] data;
    }
};
