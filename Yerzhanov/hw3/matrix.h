#include <iostream>
#include <exception>


class Row {
private:
    int num_cols_;
    int* row_;
public:
    Row(void)
        :   row_(nullptr) {}
    Row(int num_cols)
        :   row_(nullptr) 
    {
        row_ = new int[num_cols];
    }
/*    ~Proxy(void) {  delete [] row_; }

    Proxy(const Proxy&) = delete;
    void operator=(const Proxy&) {
        
    }*/

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
        return row_[j];
    }

    int operator[](int j) const {
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
        ,   matrix_(nullptr)            //
    {
        matrix_ = new Row[num_rows_];
        for (int i = 0; i < num_rows_; ++i) {
            matrix_[i] = Row(num_cols_);
        }
    }/*
    ~Matrix(void) {
        for (int i = 0; i < rows_; ++i) {
            delete [] matrix_[i];
        }
        delete [] matrix_;
    }
    
    //Следущие два метода нужны, чтобы не было коллизий в памяти. То есть если сработает конструтор копирования, то два 
    // элемента будут иметь указатель на один блок памяти, и при срабатывании конструктора этот блок будет удален дважды.
    Matrix(const Matrix&) = delete;
    void operator=(const Matrix&) = delete;
*/

    int getRows(void) { return num_rows_; }
    int getColumns(void) { return num_cols_; }

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
