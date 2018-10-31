#include <iostream>

class Matrix {
private:
    const size_t rows, cols;
    int *mx;

    class Row {
    private:
        int *line;
        size_t cols;
    public:
        explicit Row (int *line, size_t cols)
            :line(line), cols(cols)
        {}

        int& operator[](size_t n) const {
            if ((n < 0) || (n > cols - 1)) {
                throw std::out_of_range("");
            }
            return line[n];
        }

        int &operator[](size_t n) {
            if ((n < 0) || (n > cols - 1)) {
                throw std::out_of_range("");
            }
            return line[n];
        }
    };

public:
    Matrix (const size_t rows, const size_t cols)
        :rows(rows), cols(cols)
    {
        mx = static_cast<int *>(malloc(rows * cols * sizeof(int)));
    }

    ~Matrix() {
        free(mx);
    }

    size_t getRows() const {
        return rows;
    };
    size_t getColumns() const {
        return cols;
    };

    bool operator==(const Matrix &other) const {
        if (this == &other) {
            return true;
        }
        if ((rows != other.getRows()) || (cols != other.getColumns())) {
            return false;
        }
        return memcmp(mx, other.mx, rows * cols * sizeof(int)) == 0;
    }
    Row operator[](size_t m) const {
        if ((rows == 0) || (cols == 0)) {
            throw std::out_of_range("");
        }
        if ((m < 0) || (m > rows - 1)) {
            throw std::out_of_range("");
        }
        return Row(mx + m * cols, cols);
    }

    bool operator !=(const Matrix &other) {
        return !(*this == other);
    }

    Matrix &operator *=(int mul) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                mx[i * cols + j] *= mul;
            }
        }
        return *this;
    }
};
