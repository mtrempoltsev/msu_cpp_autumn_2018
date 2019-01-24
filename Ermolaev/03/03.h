#include <stdexcept>


class Matrix {
private:
    const size_t rows, cols;
    int **data;

    class ProxyClass {
    private:
        const size_t length;
        int* const data;

    public:
        ProxyClass() : length(0), data(nullptr) {}

        ProxyClass(size_t _length, int* _data) : length(_length), data(_data){}

        const int& operator[](int index) const {
            if ((index < 0) || (index >= length)) {
                throw std::out_of_range("");
            }
            return data[index];
        }

        int& operator[](int index) {
            if ((index < 0) || (index >= length)) {
                throw std::out_of_range("");
            }
            return data[index];
        }
    };

public:
    Matrix(size_t _rows = 0, size_t _cols = 0) : rows(_rows), cols(_cols) {
        if (not(rows) || not(cols)) {
            data = nullptr;
        } else {
            data = new int*[rows];
            for (int i = 0; i < rows; ++i) {
                data[i] = new int[cols];
            }
        }
    }

    size_t getRows() {
        return rows;
    }

    size_t getColumns() {
        return cols;
    }

    const ProxyClass operator[](int index) const {
        if ((index < 0) || (index >= rows)) {
            throw std::out_of_range("");
        }
        return ProxyClass(cols, data[index]);
    }

    ProxyClass operator[](int index) {
        if ((index < 0) || (index >= rows)) {
            throw std::out_of_range("");
        }
        return ProxyClass(cols, data[index]);
    }

    Matrix& operator*=(int number) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] *= number;
            }
        }
        return *this;
    }

    bool operator==(const Matrix& other) {
        if ((rows != other.rows) || (cols != other.cols)) {
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

    ~Matrix() {
        if (data != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
        }
    }
};
