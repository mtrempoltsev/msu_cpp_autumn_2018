#include <stdexcept>

class Matrix 
{
 
    int* matrix;
    
    const size_t rows;
    const size_t columns;
    
    class M_Row 
    {
    private:
        const size_t length;
        int* data;

    public:
        
        M_Row(int* data, const size_t length) : length(length), data(data) { }

        int& operator[](const size_t column) 
        {
            if (column >= length)
                throw std::out_of_range("");

            return data[column];
        }
        
        int operator[](const size_t column) const 
        {
            if (column >= length)
                throw std::out_of_range("");

            return data[column];
        }

    };

public:
    
    Matrix(const size_t rows, const size_t columns) : rows(rows) 
        , columns(columns) 
        , matrix(new int[rows * columns]) 
        { }

    ~Matrix() 
    {
        delete[] matrix;
    }

    int getRows() const
    {
        return rows;
    }

    int getColumns() const
    {
        return columns;
    }

    M_Row operator[](const size_t row)
    {
        if (row >= rows)
            throw std::out_of_range("");

        return M_Row(matrix + row*columns, columns);
    }

    const M_Row operator[](const size_t row) const 
    {
        if (row >= rows)
            throw std::out_of_range("");

        return M_Row(matrix + row*columns, columns);
    }

    const Matrix& operator*=(int number)
    {
        for (int i = 0; i < rows*columns; i++) 
            matrix[i] *= number;

        return *this;
    }
    
    
    
    bool operator==(const Matrix& matx) const
    {
        if (this == &matx)
            return true;

        if (rows != matx.rows || columns != matx.columns)
            return false;

        for (int i = 0; i < rows*columns; i++) 
        {
            if (matrix[i] != matx.matrix[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Matrix& matx) const 
    {
        return !(*this == matx);
    }


};