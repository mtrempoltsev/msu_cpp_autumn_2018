#include <exception>
#include <iostream>

class Matrix {
    size_t rows;
    size_t cols;
    int *a;
    
    class Array {
        size_t size;
        int *a;
    public:
        Array(int* start, size_t sz)
            : size(sz)
            , a(start)
        {
        }
        
        ~Array()
        {
        }
        
        int& operator[] (size_t index)
        {
            if (size <= index) {
                throw std::out_of_range("56");
            }
            return a[index];
        }
        int operator[] (size_t index) const
        {
            if (size <= index) {
                throw std::out_of_range("");
            }
            return a[index];
        }
    };
    
public:
    Matrix(size_t rs, size_t cs)
        : rows(rs)
        , cols(cs)
        , a(new int[rs * cs])
    {
    }
    
    ~Matrix()
    {
        delete[] a;
    }
    
    size_t getRows() const
    {
        return rows;
    }
    size_t getColumns() const
    {
        return cols;
    }
    
    Array operator[] (size_t index)
    {
        if (rows <= index) {
            throw std::out_of_range("");
        }
        return Array(&a[cols * index], cols);
    }
    const Array operator[] (size_t index) const
    {
        if (rows <= index) {
            throw std::out_of_range("");
        }
        return Array(&a[cols * index], cols);
    }
    
    Matrix& operator *= (int arg)
    {
        for (size_t i = 0; i < cols * rows; ++i) {
            a[i] *= arg;
        }
        return *this;
    }
    
    bool operator == (const Matrix& arg) const
    {
        if ((this->cols != arg.getColumns()) || (this->rows != arg.getRows())) {
            return false;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (a[cols * i + j] != arg[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator != (const Matrix& arg) const
    {
        return !(*this == arg);
    }
};
