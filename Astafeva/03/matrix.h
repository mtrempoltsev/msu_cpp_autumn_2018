#include <iostream>
#include <vector>

class Matrix {
    class Row {
    public:
        std::vector <int> v;
        size_t sz;
        Row() {};
        Row(int sz) : sz(sz) {
            v.resize(sz);
        };
        int& operator[](size_t i) {
            if (i >= sz || i < 0) {
                throw std::out_of_range("");
            }
            return v[i];
        }
        const int& operator[](size_t i) const {
            if (i >= sz || i < 0) {
                throw std::out_of_range("");
            }
            return v[i];
        }
    };
    std::vector <Row> m;
    size_t rows, columns;
public:
    Matrix(size_t rows, size_t columns) : rows(rows), columns(columns) {
        m.assign(rows, Row(columns));
    };
    int getRows() const {
        return rows;
    }
    int getColumns() const {
        return columns;
    }
    Row& operator[](size_t i) {
        if (i >= rows || i < 0) {
            throw std::out_of_range("");
        }
        return m[i];
    }
    const Row& operator[](size_t i) const {
        if (i >= rows || i < 0) {
            throw std::out_of_range("");
        }
        return m[i];
    }
    Matrix& operator*=(int n) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j){
                m[i][j] *= n;
            }
        }
        return *this;
    }
    bool operator==(const Matrix &M) const {
        if (rows != M.rows || columns != M.columns) {
            return 0;
        }
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j){
                if (M[i][j] != (*this)[i][j]) {
                    return 0;
                }
            }
        }
        return 1;
    }
    bool operator!=(const Matrix &M) const {
        return !(M == *this);
    }
};
