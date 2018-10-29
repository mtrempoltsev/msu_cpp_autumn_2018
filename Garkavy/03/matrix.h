#include <iostream>

class Matrix
{
    int rows;
    int cols;
    int* ptr;
public:
    Matrix(int rows, int cols)
        : rows(rows)
        , cols(cols)
    {
        // ptr = static_cast<int>(calloc(rows * cols, sizeof(int)));
        ptr = new int[rows * cols];
    }
    ~Matrix()
    {
        delete[] ptr;
    }
    int getRows() const {return rows;}
    int getColumns() const {return cols;}

    class MatrixRow
    {
        int len;
        int* ptr;
    public:
        MatrixRow(int len, int* ptr)
            : len(len)
            , ptr(ptr)
        {
        }
        int& operator[](size_t idx)
        {
            if(idx >= len) {
                throw std::out_of_range("");
            }
            return *(ptr + idx);
        }
        int operator[](size_t idx) const
        {
            if(idx >= len) {
                throw std::out_of_range("");
            }
            return *(ptr + idx);
        }
    };
    Matrix& operator*=(int factor)
    {
        for(size_t i = 0; i < rows * cols; ++i) {
            ptr[i] *= factor;
        }
        return *this;
    }
    bool operator==(const Matrix& other) const
    {
        if(ptr == other.ptr) {
            return true;
        }
        if(rows != other.rows || cols != other.cols) {
            return false;
        }
        for(size_t i = 0; i < rows * cols; ++i) {
            if(ptr[i] != other.ptr[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }
    const MatrixRow operator[](size_t row_idx) const
    {
        if(row_idx >= rows) {
            throw std::out_of_range("");
        }
        return MatrixRow(cols, ptr + row_idx * cols);
    }
    MatrixRow operator[](size_t row_idx)
    {
        if(row_idx >= rows) {
            throw std::out_of_range("");
        }
        return MatrixRow(cols, ptr + row_idx * cols);
    }
};
