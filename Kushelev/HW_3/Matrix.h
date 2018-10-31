#include <stdexcept>
#include <cstdio>
#include <iostream>

class Matrix
{
    size_t rows;
    size_t columns;
    int* Data;

    class Row
    {
        size_t columns;
        int* data;
    public:
        Row(size_t columns_num, int* row_data)
            : columns(columns_num), data(row_data)
            {}
        int& operator[](size_t second_index)
        {
            if ((second_index < columns) && (second_index >= 0))
                return data[second_index];
            throw std::out_of_range("");
        }

        const int& operator[](size_t second_index) const
        {
            if(second_index < columns)
                return data[second_index];
            throw std::out_of_range("");
        }

    };

public:
    Matrix(size_t rows_num, size_t columns_num)
        : rows(rows_num), columns(columns_num)
        {
            Data = new int[rows * columns];
        }

    size_t getRows()
    {
        return rows;
    }

    size_t getColumns()
    {
        return columns;
    }

    Row operator [](size_t first_index)
    {
        if ((first_index >= 0) && (first_index < rows))
            return Row(columns, Data + (first_index) * columns);
        throw std::out_of_range("");
    }

    const Row operator [](size_t first_index) const
    {
        if ((first_index >= 0) && (first_index < rows))
            return Row(columns, Data + (first_index) * columns);
        throw std::out_of_range("");
    }

    Matrix& operator *=(int mult) {
        for(size_t i = 0; i < columns * rows; i++)
            Data[i] *= mult;
        return *this;
    }

    bool operator ==(const Matrix& Compared) const
    {
        if((rows != Compared.rows) || (columns != Compared.columns))
            return false;
        for (int i = 0; i < rows * columns; i++)
            if (Data[i] != Compared.Data[i])
                return false;
        return true;
    }

    bool operator !=(const Matrix& Compared) const
    {
        if((rows != Compared.rows) || (columns != Compared.columns))
            return true;
        for (int i = 0; i < rows * columns; i++)
            if (Data[i] != Compared.Data[i])
                return true;
        return false;
    }

};
