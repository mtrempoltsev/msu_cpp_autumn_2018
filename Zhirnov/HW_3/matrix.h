#include <iostream>

class Vector {
    int size;
    int* vector = nullptr;

public:
    Vector(int* p_matrix, int vect_size): vector(p_matrix), size(vect_size) {};

    int operator[](int pos) const
    {
        if ((pos < size) && (pos >= 0))
            return vector[pos];
        else
            throw std::out_of_range("");
    }

    int& operator[](int pos)
    {
        if ((pos < size) && (pos >= 0))
            return vector[pos];
        else
            throw std::out_of_range("");
    }
};

class Matrix {
    int row_n, col_n;
    int** matrix = nullptr;

public:
    Matrix(int row_num = 1, int col_num = 1): row_n(row_num), col_n(col_num)
    {
        if ((row_num < 0) || (col_num < 0))
            throw std::out_of_range("");

        matrix = new int* [row_n];

        for (int j = 0; j < row_n; j++)
            matrix[j] = new int[col_n];

        for (int i = 0; i < row_n; i++)
            for (int j = 0; j < col_n; j++)
                matrix[i][j] = 0;
    }

    ~Matrix()
    {
        for (int j = 0; j < row_n; j++)
            delete[] matrix[j];

        delete[] matrix;
    }

    int getRows() const { return row_n; };

    int getColumns() const { return col_n; };

    const Vector operator[] (int pos) const
    {
        if ((pos < row_n) && (pos >= 0))
            return Vector(matrix[pos], col_n);
        else
            throw std::out_of_range("");
    }

    Vector operator[] (int pos)
    {
        if ((pos < row_n) && (pos >= 0))
            return Vector(matrix[pos], col_n);
        else
            throw std::out_of_range("");
    }

    bool operator== (const Matrix& table) const
    {

        if (this == &table)
            return true;

        if ((table.getColumns() != col_n) || (table.getRows() != row_n))
            return false;

        for (int i = 0; i < row_n; i++)
            for (int j = 0; j < col_n; j++)
                
                if (matrix[i][j] != table[i][j])
                    return false;

        return true;
    }

    bool operator!= (const Matrix& other) const
    {
        if (*this == other)
            return false;

        return true;
    }

    void operator*= (int value)
    {
        for (int i = 0; i < row_n; i++)
            for (int j = 0; j < col_n; j++)
                matrix[i][j] *= value;
    }
};

std::ostream& operator<<(std::ostream& out, const Matrix& m)
{
    for (int i = 0; i < m.getRows(); i++) {
        for (int j = 0; j < m.getColumns(); j++)
            out << m[i][j] << " ";

        out << std::endl;
    }

    return out;
};