//
//  Matrix.h
//  class-matrix
//
//  Created by Мамаев Павел on 25/10/2018.
//  Copyright © 2018 Мамаев Павел. All rights reserved.
//

#include <stdlib.h>

class Matrix {
    int *m_;
    int cols, rows;
    class Roww {
        int* cols;
        int col_size;
    public:
        Roww(int *cols, int size): cols(cols), col_size(size) {
        }
        int &operator[](size_t i) {
            if (i >= col_size) {
                throw std::out_of_range("out");
            }
            return cols[i];
        }
        int operator[](size_t i) const {
            if (i >= col_size) {
                throw std::out_of_range("out");
            }
            return cols[i];
        }
    };
public:
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        m_ = (int*)malloc(rows * cols * sizeof(int));
        if (m_ == nullptr) {
            throw std::length_error("too much elems");
        }
    }
    ~Matrix() {
        free(m_);
    }
    int getColumns() const {
        return cols;
    }
    int getRows() const {
        return rows;
    }
    Roww operator[](size_t i) {
        if (i >= rows) {
            throw std::out_of_range("out");
        }
        return Roww(m_ + cols * i, cols);
    }
    const Roww operator[](size_t i) const {
        if (i >= rows) {
            throw std::out_of_range("out");
        }
        return Roww(m_ + cols * i, cols);
    }
    Matrix& operator*=(int a) {
        for (auto i = 0; i < cols * rows; ++i) {
            m_[i] = m_[i] * a;
        }
        return *this;
    }
    bool operator==(const Matrix& x) const {
        if (cols * rows != x.cols * x.rows) {
            return false;
        }
        for (auto i = 0; i < cols * rows; ++i) {
            if (x.m_[i] != m_[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Matrix& x) const {
        return !(*this == x);
    }
};
