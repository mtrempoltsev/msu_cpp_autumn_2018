#include <cstdlib>
#include <stdexcept>

class Matrix {

    std::size_t rows;
    
    std::size_t cols;

    int *arr;

    class Row {

        const Matrix * const owner;

        std::size_t shift;
            
    public:

        Row(const Matrix *owner_, std::size_t shift_)
            : owner(owner_)
            , shift(shift_)
        { }

        const int &operator[](std::size_t j) const {
            if (j < owner->cols) {
                return owner->arr[shift + j];
            }
            throw std::out_of_range("");
        }

        int &operator[](std::size_t j) {
            if (j < owner->cols) {
                return owner->arr[shift + j];
            }
            throw std::out_of_range("");
        }

    };

public:

    Matrix(std::size_t, std::size_t);

    std::size_t getRows() const;

    std::size_t getColumns() const;

    Row operator[](std::size_t i) const;

    Row operator[](std::size_t i);

    Matrix &operator*=(int mult);

    bool operator==(const Matrix &) const;

    bool operator!=(const Matrix &) const;

    Matrix(const Matrix &) = delete;
    
    Matrix &operator=(const Matrix &) = delete;

    ~Matrix();

};

Matrix::Matrix(std::size_t rows_, std::size_t cols_)
    : rows(rows_)
    , cols(cols_)
    , arr(static_cast<int *>(std::malloc(rows_ * cols_ * sizeof(*arr))))
{
    if (!arr) {
        throw std::length_error("Not enough memory");
    }
}

std::size_t Matrix::getRows() const {
    return rows;
}

std::size_t Matrix::getColumns() const {
    return cols;
}

Matrix::Row Matrix::operator[](std::size_t i) const {
    if (i < rows) { 
        return Row(this, i * cols);
    }
    throw std::out_of_range("");
}

Matrix::Row Matrix::operator[](std::size_t i) {
    if (i < rows) { 
        return Row(this, i * cols);
    }
    throw std::out_of_range("");
}

Matrix &Matrix::operator*=(int mult) {
    for (std::size_t i = 0; i < rows * cols; ++i) {
        arr[i] *= mult; 
    }
    return *this;
}

bool Matrix::operator==(const Matrix &anoth) const {
    if (anoth.rows != this->rows ||
            anoth.cols != this->cols) {
        return false;
    }

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            if (arr[i * cols + j] != anoth[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &anoth) const {
    return !(*this == anoth);
}

Matrix::~Matrix() {
    std::free(arr);
}
