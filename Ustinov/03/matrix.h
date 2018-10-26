class Matrix {
 public:
    Matrix(size_t rows, size_t cols):
          rows(rows), cols(cols)  {
        data = new int*[rows];
        for (size_t i=0; i < rows; ++i) {
            data[i] = new int[cols];
        }
    }

    ~Matrix() {
        for (size_t j=0; j < rows; ++j) {
            delete[] data[j];
        }
        delete[] data;
     }

    class Array {
     public:
        Array(int** data, size_t r, size_t cols):
              data(data), r(r), cols(cols) {  }
        ~Array() {  }
        int& operator[](size_t c) {
            if (c >= cols) throw std::out_of_range("");
            return data[r][c];
        }
        int operator[](size_t c) const {
          if (c >= cols) throw std::out_of_range("");
          return data[r][c];
        }
     private:
       int** data;
       const size_t r;
       const size_t cols;
    };

    Array operator[](size_t r) {
        if (r >= rows) throw std::out_of_range("");
        return Array(data, r, cols);
    }

    const Array operator[](size_t r) const {
        if (r >= rows) throw std::out_of_range("");
        return Array(data, r, cols);
    }

    int getRows() const {
        return rows;
    }

    int getColumns() const {
        return cols;
    }

    Matrix& operator*=(int num) {
        for (size_t i=0; i < rows; ++i) {
            for (size_t j=0; j < cols; ++j) {
                data[i][j] *= num;
            }
        }
        return *this;
    }

    bool operator==(const Matrix& m2) const {
        if (m2.getRows() != rows) return false;
        else if (m2.getColumns() != cols) return false;
        else if (this == &m2) return true;
        for (size_t i=0; i < rows; ++i) {
            for (size_t j=0; j < cols; ++j) {
                if (data[i][j] != m2[i][j]) return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& m2) const {
        return !(*this == m2);
    }

 private:
    const size_t rows;
    const size_t cols;
    int ** data;
};
