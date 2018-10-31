#pragma once

class Matrix
{
private:
    
    int *mat;
    size_t rows, cols;
    
    class Proxy 
    {
    private:
        int *row;
        size_t len;
    public:
        Proxy(int *row, size_t len): row(row),
                                     len(len){}
        
        int& operator[] (size_t index) 
        {
            if (index < len) {
                return row[index];   
            }
            throw std::out_of_range(""); 
        }
        
        const int& operator[](size_t index) const
        {
            if (index < len) {
                return row[index];   
            }
            throw std::out_of_range(""); 
        }
    };
    
public:
    
    Matrix(size_t rows, size_t cols) : rows(rows), 
                                       cols(cols), 
                                       mat(new int[cols*rows]){}
    
    ~Matrix() 
    {
        delete[] mat;    
    }
    
    size_t getRows() const 
    {
        return rows;   
    }
    
    size_t getColumns() const 
    {
        return cols;           
    }
    
    bool operator==(const Matrix& other) const
    {
        if (this == &other)
            return true;
        
        if (cols != other.getRows() || rows != other.getColumns()) {
            return false;
        }
        
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; j++)
                if (mat[i*rows + j] != other[i][j]) {
                    return false;   
                }
        
        return true;
    }
    
    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }
    
    const Proxy operator[](size_t row) const {
        if (row < rows) {
            return Proxy(mat + cols*row, cols);
        }
        throw std::out_of_range(""); 
    }
    
    Proxy operator[](size_t row) {
        if (row < rows) {
            return Proxy(mat + cols*row, cols);
        }
        throw std::out_of_range(""); 
    }
 
    Matrix& operator*=(int x) {
        for (int i = 0; i < rows*cols; ++i)
            mat[i] *= x;        
        return *this;
    }
    
};
