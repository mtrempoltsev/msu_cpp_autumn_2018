#include <cstddef>
#include <iostream>
#include <cassert>

class Row {
    size_t length;
    int* row_data;
public:
    Row(int* begin_address, size_t len) : length(len), 
                                          row_data (begin_address) { }

    int& operator[] (size_t x)
    {
        if ((x < length) && (x >= 0))
            return row_data[x];
        else
            throw std::out_of_range("");
    }

    int operator[] (size_t x) const 
    {
        if ((x < length) && (x >= 0))
            return row_data[x];
        else
            throw std::out_of_range("");
    }

    ~Row() { }
};

class Matrix {
    size_t height;
    size_t length;
    int* data;
public:
    Matrix(size_t hgt, size_t len) : height(hgt), length(len),
                                     data (new int[height*length]) {
        for (size_t x = 0; x < height * length; x++)
                data[x] = 0;
    }

    size_t getRows() const { return height; }
    size_t getColumns() const { return length; }

    const Row operator[](size_t y) const
    {
        if ((y < height) && (y >= 0)) {
            Row tmp = Row(&data[length * y], length);
            return tmp;
        } else {
            throw std::out_of_range("");
        }
    }
    
    Row operator[](size_t y)
    {
        if ((y < height) && (y >= 0)) {
            Row tmp = Row(&data[length * y], length);
            return tmp;
        } else {
            throw std::out_of_range("");        
        }
    }

    Matrix& operator*=(int multiplier)
    {
        for (size_t x = 0; x < height * length; x++)
            data[x] *= multiplier;
        return *this;
    }

    bool operator==(const Matrix& other) const
    {
        if (getRows() != other.getRows() || getColumns() != other.getColumns())
            return false;
        for (size_t y = 0; y < height; y++)
            for (size_t x = 0; x < length; x++)
                if (data[y * length + x] != other[y][x])
                    return false;
        return true;
    }

    bool operator!= (const Matrix& other) const
    { return !(*this == other); }

    ~Matrix() { 
        delete[] data;
    }
};