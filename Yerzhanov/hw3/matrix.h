#include <iostream>
#include <exception>
#include <new>

class Row {
private:
    int num_cols_;
    int* row_;
public:
    Row(void)
        :   num_cols_(0)
        ,   row_(nullptr) {}
    
    Row(int num_cols)
        :   num_cols_(num_cols)
        ,   row_(nullptr) 
    {
        row_ = new int[num_cols];
    }
    
    ~Row(void) {
        delete [] row_;
    }

    Row& operator=(Row& m) {
        num_cols_ = m.num_cols_;
        row_ = new (m.row_) int[num_cols_];
        m.row_ = nullptr;    // Я честно не знаю на сколько это костыльно
    }

    Row& operator=(const Row& m) {
        num_cols_ = m.num_cols_;
        row_ = new (m.row_) int[num_cols_];
    }

    Row& operator*=(int a) {
        for (int i = 0; i < num_cols_; ++i) {
            row_[i] *= a;
        }
        return *this;
    }

    bool operator==(const Row& r) {
        for (int i = 0; i < num_cols_; ++i) {
            if (row_[i] != r.row_[i]) {
                return false;
            }
        }
        return true;
    }

    int& operator[](int j) {
        if (j < 0 || j >= num_cols_) {
            throw std::out_of_range("");
        }
        return row_[j];
    }

    int operator[](int j) const {
        if (j < 0 || j >= num_cols_) {
            throw std::out_of_range("");
        }
        return row_[j];
    }
};


class Matrix {
private:
    int num_rows_, num_cols_;
    Row *matrix_;
public:
    Matrix(int num_rows, int num_cols)
        :   num_rows_(num_rows)
        ,   num_cols_(num_cols) 
    {
        matrix_ = new Row[num_rows_];
        for (int i = 0; i < num_rows_; ++i) {
            Row tmp = Row(num_cols_);
            matrix_[i] = tmp;
        }
    }
    ~Matrix(void) {
        delete [] matrix_;
    }

    int getRows(void) const { return num_rows_; }
    int getColumns(void) const { return num_cols_; }

    Matrix& operator*=(int a) {
        for (int i = 0; i < num_rows_; ++i) {
            matrix_[i] *= a;
        }
        return *this;
    }

    bool operator==(const Matrix& m) const {
        if (&m == this) {
            return true;
        }
        if (num_rows_ != m.num_rows_ || num_cols_ != m.num_cols_) {
            return false;
        }
        for (int i = 0; i < num_rows_; ++i) {
            if (!(matrix_[i] == m.matrix_[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& m) const {
        return !(*this == m);
    }

    Row& operator[](int i) {
        if (i < 0 || i >= num_rows_) {
            throw std::out_of_range("");
        }
        return matrix_[i];
    }

    const Row& operator[] (int i) const {
        if (i < 0 || i >= num_rows_) {
            throw std::out_of_range("");
        }
        return matrix_[i];
    }
};
