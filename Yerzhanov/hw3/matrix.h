#include <iostream>
#include <vector>
#include <exception>


class Proxy {
private:
    std::vector<int>* data_;
public:
    Proxy() {
        data_ = nullptr;
    }

    Proxy(std::vector<int>& data) {
        data_ = &data;
    }

    int& operator[](int j) {
        if (j < 0 || static_cast<size_t>(j) >= (*data_).size()) {
            throw std::out_of_range("");
        }
        return (*data_)[j];
    }
    void print(void) {
        for (size_t i = 0; i < (*data_).size(); ++i) {
            std::cout << (*data_)[i] << '\t';
        }
        std::cout << '\n';
    }

};

class Proxy2 {
private:
    std::vector<int> data_;
public:
    Proxy2() {}

    Proxy2(const std::vector<int>& data) {
        data_ = data;
    }

    int operator[](int j) const {
        if (j < 0 || static_cast<size_t>(j) >= data_.size()) {
            throw std::out_of_range("");
        }
        return data_[j];
    }
};

class Matrix {
private:
    int rows_, columns_;
    Proxy obj; 
    Proxy2 obj2;
    std::vector<std::vector<int>> matrix_;

public:
    Matrix(int a, int b) {
        rows_ = a;
        columns_ = b;
        std::vector<int> tmp(b, 1);
        std::vector<std::vector<int>> matrix(a, tmp);
        matrix_ = matrix;
    }

    int getRows(void) const { return rows_; }
    int getColumns(void) const { return columns_; }

    bool operator==(const Matrix& m) const {
        if (this == &m) {
            return true;
        }
        if (rows_ != m.rows_ || columns_ != m.columns_) {
            return false;
        }
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < columns_; ++j) {
                if (matrix_[i][j] != m.matrix_[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& m) const {
        return !(*this == m);
    }

    Matrix& operator*=(int a) {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < columns_; ++j) {
                matrix_[i][j] *= a;
            }
        }
        return *this;
    }
    
    
    Proxy& operator[](int i) {
        if (i >= rows_ || i < 0) {
            throw std::out_of_range("");
        }
        obj = Proxy(matrix_[i]);
        return obj;
    }

    const Proxy2 operator[](int i) const {
        if (i >= rows_ || i < 0) {
            throw std::out_of_range("");
        }
        return Proxy2(matrix_[i]);
    }
};
