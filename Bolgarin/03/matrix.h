#include <stdexcept>

class Matrix {
    using Size = size_t;

    Size rows_;
    Size columns_;
    int* matrix_;

    class MatrixRow {
        Size length_;
        int* data_;

    public:
        MatrixRow(int* data, Size length) : length_(length), data_(data) { }

        int operator[](Size column) const {
            if (column >= length_)
                throw std::out_of_range("");

            return data_[column];
        }

        int& operator[](Size column) {
            if (column >= length_)
                throw std::out_of_range("");

            return data_[column];
        }
    };

public:
    Matrix(Size rows, Size columns) 
        : rows_(rows) 
        , columns_(columns) 
        , matrix_(new int[rows_ * columns_]) 
        { }

    ~Matrix() {
        delete[] matrix_;
    }

    Size getRows() const {
        return rows_;
    }

    Size getColumns() const {
        return columns_;
    }

    MatrixRow operator[](Size row) {
        if (row >= rows_)
            throw std::out_of_range("");

        return MatrixRow(matrix_ + row*columns_, columns_);
    }

    const MatrixRow operator[](Size row) const {
        if (row >= rows_)
            throw std::out_of_range("");

        return MatrixRow(matrix_ + row*columns_, columns_);
    }

    bool operator==(const Matrix& matx) const {
        if (this == &matx)
            return true;

        if (rows_ != matx.rows_ || columns_ != matx.columns_)
            return false;

        for (int i = 0; i < rows_*columns_; i++) {
            if (matrix_[i] != matx.matrix_[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Matrix& matx) const {
        return !(*this == matx);
    }

    const Matrix& operator*=(int number) {
        for (int i = 0; i < rows_*columns_; i++) 
            matrix_[i] *= number;

        return *this;
    }
};