#include <cstddef>
#include <stdexcept>

class Matrix {
    const size_t rows;
    const size_t cols;
    int *data;
    
    class Row {
        const size_t row_ind;
        const size_t cols;
        int *data;

    public:

        Row(int *data, const size_t row_ind, const size_t cols) :
            data(data), row_ind(row_ind), cols(cols) {}

        int& operator[](const size_t col_ind)
        {
            if (col_ind >= cols) {
                throw std::out_of_range("");
            }
            return data[row_ind * cols + col_ind];
        }

        const int operator[](const size_t col_ind) const
        {
            if (col_ind >= cols) {
                throw std::out_of_range("");
            }
            return data[row_ind * cols + col_ind];
        }
    };

public:

    Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols)
    {
        data = new int[rows * cols];
    }

    ~Matrix()
    {
        delete data;
    }

    size_t getRows() const
    {
        return rows;
    }

    size_t getColumns() const
    {
        return cols;
    }

    Row operator[](const size_t row_ind) const
    {
        if (row_ind >= rows) {
            throw std::out_of_range("");
        }
        return Row(data, row_ind, cols);
    }

    Matrix& operator*=(int val)
    {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                data[i * cols + j] *= val;
            }
        }
        return *this;
    }

    bool operator==(const Matrix &m) const
    {
        if (this == &m) {
            return true;
        }
        
        if (this->rows != m.rows || this->cols != m.cols) {
            return false;
        }

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (data[i * cols + j] != m[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix &m) const
    {
        return !(*this == m);
    }

};
