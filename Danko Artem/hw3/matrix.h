#include <malloc.h>


class Matrix {
    int *matr;
    int cols, rows;
    mutable struct Subclass{
        int* cols;
        int c_length;
        int& operator[](size_t i) {
            if (i >= c_length)
                throw std::out_of_range("out");
            return cols[i];
        }
        const int& operator[](size_t i) const {
            if (i >= c_length)
                throw std::out_of_range("out");
            return cols[i];
        }
    } sub_for_ac;
public:
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        matr = (int*)malloc(rows * cols * sizeof(int));
        if (matr == nullptr)
            throw std::length_error("too much elems");
        sub_for_ac.c_length = cols;

    }
    int getColumns() {
        return cols;
    }
    int getRows() {
        return rows;
    }
    Subclass& operator[](size_t i) {
        if (i >= rows)
            throw std::out_of_range("out");
        sub_for_ac.cols = matr + cols * i;
        return sub_for_ac;
    }
    const Subclass& operator[](size_t i) const {
        if (i >= rows)
            throw std::out_of_range("out");
        sub_for_ac.cols = matr + cols * i;
        return sub_for_ac;
    }
    const Matrix& operator*=(const int& a) {
        for (auto i = 0; i < cols * rows; ++i)
            matr[i] = matr[i] * a;
        return *this;
    }
    bool operator==(const Matrix& x) const {
        if (cols * rows != x.cols * x.rows)
            return false;
        for (auto i = 0; i < cols * rows; ++i) {
            if (x.matr[i] != matr[i])
                return false;
        }
        return true;
    }
    bool operator!=(const Matrix& x) const {
        return !(*this == x);
    }
};

