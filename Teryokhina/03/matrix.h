#include <cstdio>
#include <stdexcept>

class Matrix{
    size_t rows;
    size_t columns;
    size_t matrixSize;
    int *matrixData;

    class Row
    {
        size_t columns;
        int *rowData;
    public:
        Row(int *row_start, size_t columns)
        : columns(columns), rowData(row_start) {}

        const int& operator[](size_t column_pos) const
        {
            if(column_pos < columns)
                return rowData[column_pos];
            throw std::out_of_range("");
        }

        int& operator[](size_t column_pos)
        {
            if(column_pos < columns)
                return rowData[column_pos];
            throw std::out_of_range("");
        }
    };

public:
    Matrix(size_t rows = 0, size_t columns = 0)
        : rows(rows), columns(columns), matrixSize(rows * columns)
    {
        matrixData = new int[matrixSize];
    }

    ~Matrix()
    {
        delete[] matrixData;
    }

    size_t getRows() const
    {
        return rows;
    }

    size_t getColumns() const
    {
        return columns;
    }

    bool operator==(const Matrix& other) const
    {
        if((rows != other.rows) || (columns != other.columns))
            return false;
        for (int i = 0; i < matrixSize; ++i)
            if (matrixData[i] != other.matrixData[i])
                return false;
        return true;
    }
    
    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }

    Matrix& operator*=(int num) {
        for(size_t i = 0; i < matrixSize; ++i)
            matrixData[i] *= num;
        return *this;
    }

    const Row operator[](size_t row_pos) const
    {
        if (row_pos < rows)
            return Row(matrixData + row_pos * columns, columns);
        throw std::out_of_range("");
    }
    
    Row operator[](size_t row_pos)
    {
        if (row_pos < rows)
            return Row(matrixData + row_pos * columns, columns);
        throw std::out_of_range("");
    }
};