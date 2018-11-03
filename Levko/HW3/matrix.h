class Matrix {

    using Size = size_t;

private:

    Size rows;
    Size columns;
    int** data;
    
    class Matrix2d {
    private:
        Size width;
        int* const data;

    public:    
        Matrix2d(Size number_of_colomns, int* value)
            : width(number_of_colomns)
            , data(value)
            {}
        
        int& operator[](Size colomn) {
            if (colomn < 0 || colomn >= this->width) {
                throw std::out_of_range("");
            }
            return data[colomn];
        }
        
        const int& operator[](Size colomn) const {
            if (colomn < 0 || colomn >= this->width) {
                throw std::out_of_range("");
            }
            return data[colomn];
        }
    };
    
public:
    
    Matrix(Size number_of_rows, Size number_of_columns)
        : rows(number_of_rows)
        , columns(number_of_columns)
        {
            data = new int*[rows];
            for (Size i = 0; i < rows; ++i) {
                data[i] = new int[columns];
            }
        }
        
    ~Matrix() {
        for (Size i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
    
    Size getRows() {
        return this->rows;
    }
    
    Size getColumns() {
        return this->columns;
    }
    
    Matrix2d operator[](Size index) {
        if (index < 0 || index >= this->rows) throw std::out_of_range("");
        return Matrix2d(this->columns, this->data[index]);
    }
    
    const Matrix2d operator[](Size index) const {
        if (index < 0 || index >= this->rows) throw std::out_of_range("");
        return Matrix2d(this->columns, this->data[index]);
    }
    
    
    bool operator==(const Matrix& another_matrix) {        
        if(this->rows != another_matrix.rows
            || this->columns != another_matrix.columns) return false;
        
        for(Size j = 0; j < this->columns; ++j) {
            for( Size i = 0; i < this->rows; ++i) {
                if(this->data[i][j] != another_matrix[i][j]) return false;
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix& another_matrix) {
        return !(*this == another_matrix);
    }
    
    Matrix &operator*=(int lambda) {
        for(Size j = 0; j < this->columns; ++j) {
            for( Size i = 0; i < this->rows; ++i) {
                this->data[i][j] *= lambda;
            }
        }
    return *this;
    }
    
};
