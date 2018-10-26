#include <iostream>
#include <vector>
#include <exception>
class Matrix
{
    size_t rows;
    size_t columns;
    std::vector<std::vector<int>> matrix;

    class Matrix_support
    {
        size_t i;
        Matrix *mat;
        const Matrix *const_mat;

    public:
        Matrix_support(Matrix *m, size_t coord): i(coord), mat(m) {}
        Matrix_support(const Matrix *m, size_t coord): i(coord), const_mat(m) {}
        int& operator[](size_t j)
        {
            return (*mat).getElem(i, j);
        }
        int operator[](size_t j) const
        {
            return (*const_mat).getElem(i, j);
        }
    };

public:
    Matrix(size_t r, size_t c): rows(r), columns(c)
    {
        matrix.resize(r);
        for(size_t i = 0; i < r; i++)
        {
            matrix[i].resize(c);
            for(size_t j = 0; j < c; j++)
                matrix[i][j] = 0;
        }
    }
    bool operator ==(const Matrix& m) const
    {
        return (*this).matrix == m.matrix;
    }
    bool operator !=(const Matrix& m) const
    {
        return (*this).matrix != m.matrix;
    }
    Matrix_support operator[] (size_t i)
    {
        return Matrix_support(this, i);
    }
    Matrix_support operator[] (size_t i) const
    {
        return Matrix_support(this, i);
    }
    const Matrix& operator *=(int op)
    {
        for(size_t i = 0; i < rows; i++)
            for(size_t j = 0; j < columns; j++)
                matrix[i][j] *= op;
        return (*this);
    }
    int& getElem(size_t i, size_t j)
    {
        if(i >= rows || j >= columns)
            throw std::out_of_range("");
        return  matrix[i][j];
    }
    int getElem(size_t i, size_t j) const
    {
        if(i >= rows || j >= columns)
            throw std::out_of_range("");
        return  matrix[i][j];
    }
    size_t getRows() const
    {
        return rows;
    }

    size_t getColumns() const
    {
        return  columns;
    }
};
