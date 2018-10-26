#include <iostream>
#include <cstring>

using namespace std;

class Matrix {
    class Row {
        int *row;
        int col_number;

    public:
        explicit Row(int *row, int col_size) : row(row), col_number(col_size) {}

        int operator[](size_t col) const {
            if (col >= col_number)
                throw std::out_of_range("");
            return row[col];
        }

        int &operator[](size_t col){
            if (col >= col_number)
                throw std::out_of_range("");
            return row[col];
        }
    };

    const int rows;
    const int cols;
    int *data;

public:
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        data = static_cast<int *>(malloc(rows * cols * sizeof(int)));
    }

    ~Matrix(){
        free(data);
    }

    const Row operator[](int row) const {
        if (row >= rows)
            throw std::out_of_range("");
        return Row(data + row *  cols, cols);
    }

    Row operator[](int row){
        if (row >= rows)
            throw std::out_of_range("");
        return Row(data + row *  cols, cols);
    }

    bool operator==(const Matrix &other) const {
        if ((cols == other.cols) && (rows == other.rows))
            return !bool(memcmp(data, other.data, rows * cols * sizeof(int)));

        return false;
    }

    bool operator!=(const Matrix &other) const {
        return !(this == &other);
    }

    Matrix const &operator*=(const int multiplier) const {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                data[i * cols + j] *= multiplier;
        return *this;
    }

    int getRows() const {
        return rows;
    }

    int getColumns() const {
        return cols;
    }
};
