#pragma once


class Matrix{
private:
    const size_t rows;
    const size_t cols;
    int* array;
    
    class Proxy{
    private:
        int* data;
        size_t size;
    public:
        Proxy(int* data, size_t size) : data(data), size(size){}
        int operator[](size_t el) const{
            if(el >= size)
            throw std::out_of_range("");
            return data[el];
        }
        int& operator[](size_t el){
            if(el >= size)
            throw std::out_of_range("");
            return data[el];
        }
    };
public:
    Matrix(const size_t rows, const size_t cols)
        :rows(rows),
        cols(cols),
        array(new int[rows * cols]){}
    ~Matrix(){
        delete[] array;
    }
    Matrix& operator*=(size_t el){
        for(auto i = 0; i < rows * cols; i++)
            array[i] *= el;
        return *this;
    }
    Proxy operator[](size_t el){
        if(el >= rows)
            throw std::out_of_range("");
        return Proxy(array + el * cols, cols);
    }
    const Proxy operator[](size_t el) const{
        if(el >= rows)
            throw std::out_of_range("");
        return Proxy(array + el * cols, cols);
    }
    bool operator==(const Matrix& other){
        if(this == &other)
            return true;
        for(auto i = 0; i < rows * cols; i++)
            if(array[i] != other.array[i])
                return false;
        return true;
    }
    bool operator!=(const Matrix& other){
        return !(*this == other);
    }
    auto getRows() const{
        return rows;
    }
    auto getColumns() const{
        return cols;
    }
};
