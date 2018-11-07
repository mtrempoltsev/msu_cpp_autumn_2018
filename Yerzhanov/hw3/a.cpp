#include <iostream>


class Row {
private:
    int num_cols_;
    int* data_;
public:
    Row(void) 
        :   num_cols_(0)
        ,   data_(nullptr) {}
    Row(int num_cols, int bias)
        :   num_cols_(num_cols)
        ,   data_(nullptr)
    {
        data_ = new int[num_cols_];
        for (int i = 0; i < num_cols_; ++i) {
            data_[i] = bias + i;
        }
    }

    ~Row(void) {
        delete[] data_;
    }

    void print(void) {
        for (int i = 0; i < num_cols_; ++i) {
            std::cout << data_[i] << '\t';
        }
        std::cout << '\n';
    }
};


class Matrix {
private:
    int num_rows_, num_cols_;
    Row* data_;
public:
    Matrix(int num_rows, int num_cols)
        :   num_rows_(num_rows)
        ,   num_cols_(num_cols)
        ,   data_(nullptr)
    {
        data_ = new Row[num_rows_];
        for (int i = 0; i < num_rows_; ++i) {
            data_[i] = Row(num_cols_, i);
        }
    }

    void print(void) {
        for (int i = 0; i < num_rows_; ++i) {
                data_[i].print();
        }
    }
};


int main(void) {
    Matrix m(3, 5);
    m.print();
    return 0;
}
