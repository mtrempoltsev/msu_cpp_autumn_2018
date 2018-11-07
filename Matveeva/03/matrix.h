#include<iostream>
#include<cassert>
#include<stdexcept>
using namespace std;

class Matrix{
    class Row{
        size_t len;
        int* line;
    public:
        Row(int* l, size_t n): line(l), len(n){}
        const int operator[](size_t num_of_col) const
        {
            if (num_of_col >= len)
                throw out_of_range("");
            return line[num_of_col];
        }
        int& operator[](size_t num_of_col)
        {
            if (num_of_col >= len)
                throw out_of_range("");
            return line[num_of_col];
        }
    };
    size_t cols;
    size_t rows;
    int* data;
public:
    Matrix(size_t num_rows, size_t num_cols): rows(num_rows), cols(num_cols)
    {
        data = new int [num_cols * num_rows];
    }
    ~Matrix()
    {
        delete[] data;
    }
    const Row operator[](size_t num_of_row) const
    {
        if (num_of_row >= rows)
            throw out_of_range("");
        return Row(data + num_of_row * cols, cols);
    }
    Row operator[](size_t num_of_row)
    {
        if (num_of_row >= rows)
            throw out_of_range("");
        return Row(data + num_of_row * cols, cols);
    }
    size_t getRows()
    {
        return rows;
    }
    size_t getColumns()
    {
        return cols;
    }
    Matrix& operator*=(int m)
    {
        size_t s = cols * rows;
        for (size_t i = 0; i < s; i ++)
        {
            data[i] *= m;
        }
        return *this;
    }
    bool operator== (const Matrix& other)
    {
        if ((rows != other.rows) || (cols != other.cols))
            return false;
        for (size_t i = 0; i < rows; i ++)
        {
            for (size_t j = 0; j < cols; j ++)
            {
                if (data[i * cols + j] != other[i][j])
                return false;
            }
        }
        return true;
    }
    bool operator != (Matrix other)
    {
        return !(*this == other);
    }
};
