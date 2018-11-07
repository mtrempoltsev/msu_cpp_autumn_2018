#include<iostream>

class Row {
    int *row;
    int len;

    public:
        Row(int *row, int len) : row(row), len(len) {}

        int operator[] (size_t num) const {
            if (num >= len) 
                throw std::out_of_range("");
            return row[num];
        }

        int &operator[] (size_t num) {
            if (num >= len) 
                throw std::out_of_range("");
            return row[num];
        }
};

class Matrix {
    const int rows;
    const int cols;
    int **matrix;

    public:
        Matrix(int rows1, int cols1) :rows(rows1), cols(cols1) {
            matrix = new int *[rows];
            for (int i = 0; i < rows; ++i) {
                matrix[i] = new int [cols];
            }
        }

        ~Matrix() {
            for (int i = 0; i < rows; ++i) {
                delete[] matrix[i];
            }
            delete[] matrix;
        }
        
        int getRows() const {
            return rows;
        }
        
        int getColumns() const {
            return cols;
        }

        const Row operator[](int num) const {
            if (num >= rows) 
                throw std::out_of_range("");
            return Row(matrix[num], cols);
        }

        Row operator[](int num) {
            if (num >= rows) 
                throw std::out_of_range("");
            return Row(matrix[num], cols);
        }

        Matrix &operator*=(const int multiplier) {
            for (int i = 0; i < rows; ++i) 
               for (int j = 0; j < cols; ++j) 
                  matrix[i][j] *= multiplier;
            return *this;
        }

        bool operator==(const Matrix &second) const {
            if ((cols == second.cols) && (rows == second.rows)) {
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j) 
                       if (matrix[i][j] != second.matrix[i][j]) {
                            return false;
                       }
                return true;
            }
            else return false;
        }

        bool operator!=(const Matrix &second) const {
            return !(this == &second);
        }
};
