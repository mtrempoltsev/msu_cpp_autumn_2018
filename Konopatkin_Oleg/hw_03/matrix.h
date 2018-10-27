#include <stdexcept>


class Matrix {
private:
    const size_t rows, cols;
    int **data;

    class Matrix_line {
    private:
        const size_t len;
        int* const data;

    public:
        Matrix_line() : len(0), data(nullptr) {}

        Matrix_line(size_t _len, int* _data) : len(_len), data(_data){}

        const int& operator[](int index) const {
            if (index < 0 || index >= len) {
                throw std::out_of_range("");
            }
            return data[index];
        }

        int& operator[](int index) {
            if (index < 0 || index >= len) {
                throw std::out_of_range("");
            }
            return data[index];
        }
    };

public:
    Matrix(size_t _rows, size_t _cols) : rows(_rows), cols(_cols) {
        if (rows * cols == 0) {
            data = nullptr;
        } else {
            data = new int*[rows];
            for (int i = 0; i < rows; ++i) {
                data[i] = new int[cols];
            }
        }
    }

    ~Matrix() {
        if (data != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
        }
    }

    size_t getRows() {
        return rows;
    }

    size_t getColumns() {
        return cols;
    }

    bool operator==(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            return false;
        }

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (data[i][j] != other.data[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Matrix& other) {
        return !(*this == other);
    }

    Matrix& operator*=(int scalar) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }

    const Matrix_line operator[](int index) const {
        if (index < 0 || index >= rows) {
            throw std::out_of_range("");
        }
        return Matrix_line(cols, data[index]);
    }

    Matrix_line operator[](int index) {
        if (index < 0 || index >= rows) {
            throw std::out_of_range("");
        }
        return Matrix_line(cols, data[index]);
    }
};
