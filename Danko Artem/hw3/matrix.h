#include <malloc.h>


class Matrix {
    int *matr;
    int cols, rows;
    class Subclass{
        int* const cols;
        const int c_length;
    public:
        Subclass(int* cols, const int c_length): cols(cols), c_length(c_length){}
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
    };
public:
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        matr = (int*)malloc(rows * cols * sizeof(int));
        if (matr == nullptr)
            throw std::length_error("too much elems");
    }
    ~Matrix() {
        free(matr);
    }
    int getColumns() {
        return cols;
    }
    int getRows() {
        return rows;
    }
    Subclass operator[](size_t i) {
        if (i >= rows)
            throw std::out_of_range("out");
        return Subclass(matr + cols * i, cols);
    }
    const Subclass operator[](size_t i) const {
        if (i >= rows)
            throw std::out_of_range("out");
        return Subclass(matr + cols * i, cols);
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
