#include <vector>
#include <exception>
using namespace std;

class Row {
private:
    vector<int> data;
    const size_t size;
private:
    bool is_valid_idx(size_t i) const;
public:
    Row (size_t size_);
    const int& operator[] (size_t i) const;
    int& operator[] (size_t i);
};

const int& Row::operator[] (size_t i) const {
    if (!is_valid_idx(i)) {
        throw std::out_of_range("");
    }
    return data[i];
}

int& Row::operator[] (size_t i) {
    if (!is_valid_idx(i)) {
        throw std::out_of_range("");
    }
    return data[i];
}

bool Row::is_valid_idx(size_t i) const {
    return i >= 0 && i < size;
}

Row::Row (size_t size_) : size(size_) {
    data.assign(size, 0);
};

class Matrix {
private:
    const size_t rows;
    const size_t cols;
    std::vector<Row> data;
private:
    bool is_valid_row(size_t i) const {
        return i >= 0 && i < rows;
    }
public:
    explicit Matrix(size_t rows_, size_t cols_);
    size_t getRows() const;
    size_t getColumns() const;
    const Row& operator[] (size_t i) const;
    Row& operator[] (size_t i);
    const Matrix& operator*= (int n);
    bool operator== (const Matrix& rhs_mat) const; 
    bool operator!= (const Matrix& rhs_mat) const; 
};

Matrix::Matrix(size_t rows_, size_t cols_) : rows(rows_), cols(cols_) {
    data = std::vector<Row> (cols, Row(cols));
}

size_t Matrix::getRows() const { return rows; }

size_t Matrix::getColumns() const { return cols; }

const Row& Matrix::operator[] (size_t i) const {
    if (!is_valid_row(i)) {
        throw std::out_of_range("");
    }
    return data[i];
}

Row& Matrix::operator[] (size_t i) {
    if (!is_valid_row(i)) {
        throw std::out_of_range("");
    }
    return data[i];
}

const Matrix& Matrix::operator*= (int n) {
    for (size_t i = 0; i < getRows(); i++) {
        for (size_t j = 0; j < getColumns(); j++) {
            data[i][j] *= n;
        }
    }
    return *this;
}

bool Matrix::operator==(const Matrix& rhs_mat) const {
    if (this->getRows() != rhs_mat.getRows() || this->getColumns() != rhs_mat.getColumns()) {
        return false;
    }
    for (size_t i = 0; i < rhs_mat.getRows(); i++) {
        for (size_t  j = 0; j < rhs_mat.getColumns(); j++) {
            if ((*this)[i][j] != rhs_mat[i][j]) return false;
        }
    }
    return true;
}

bool Matrix::operator!= (const Matrix& rhs_mat) const {
    return !(*this == rhs_mat);
}