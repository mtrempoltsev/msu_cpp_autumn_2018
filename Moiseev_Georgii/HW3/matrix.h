#include <cassert>
#include <exception>
#include <cstring>

class Matrix
{
private:
    class proxy;
public:
    Matrix(size_t _rows, size_t _columns) : rows(_rows), columns(_columns), data(nullptr)
    {
        if (rows > 0 && columns > 0)
            data = new int[rows * columns];
    }

    const proxy operator[](size_t rowInd) const
    {
        return proxy(rowInd, this);
    }

    proxy operator[](size_t rowInd)
    {
        return proxy(rowInd, this);
    }

    Matrix& operator*=(int mult)
    {
        for (size_t i = 0; i < rows * columns; i++)
            data[i] *= mult;

        return *this;
    }

    bool operator==(const Matrix& other) const
    {
        if (this == &other)
            return true;

        if (this->rows != other.rows || this->columns != other.columns)
            return false;

        return memcmp(this->data, other.data, rows * columns) == 0;
    }

    bool operator!=(const Matrix& other) const
    {
        return !((*this) == other);
    }

    ~Matrix()
    {
        if (data != nullptr)
            delete[] data;
    }

    int getRows()
    {
        return rows;
    }

    int getColumns()
    {
        return columns;
    }

private:
    int *data;
    size_t rows;
    size_t columns;

    class proxy
    {
    public:
        proxy(size_t _rowInd, const Matrix* _matr) : rowInd(_rowInd), matr(_matr) {}

        const int operator[](size_t colInd) const
        {
            if (rowInd >= matr->rows || colInd >= matr->columns)
                throw std::out_of_range("");
            return matr->data[rowInd * matr->columns + colInd];
        }

        int& operator[](size_t colInd)
        {            
            if (rowInd >= matr->rows || colInd >= matr->columns)
                throw std::out_of_range("");
            return matr->data[rowInd * matr->columns + colInd];
        }
    private:
        size_t rowInd;
        const Matrix* matr;
    };
};