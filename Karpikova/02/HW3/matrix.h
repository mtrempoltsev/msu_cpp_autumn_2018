#include <stdexcept>

using namespace std;

class Matrix
{
    int rows, cols;
    int* mtx;

    class Proxy
    {
        int *arr;
        int num_of_elem;
    public:
        Proxy(int* arr, int col) : arr(arr), num_of_elem(col) {}
        int& operator[](int index)
        {
            if (index >= num_of_elem) throw std::out_of_range("");
            return arr[index];
        }
        const int& operator[](int index) const
        {
            if (index >= num_of_elem) throw std::out_of_range("");
            return arr[index];
        }
    };
public:
    Matrix(int rows, int cols)
        : rows(rows)
        ,cols(cols)
        ,mtx(new int[rows * cols])
    {
    }
    ~Matrix()
    {
        delete []mtx;
    }
    int getRows()
    {
        return rows;
    }
    int getColumns()
    {
        return cols;
    }
    Proxy operator[](int i)
    {
        if (i >= rows) throw std::out_of_range("");
        return Proxy(mtx + cols * i, cols);
    }
    const Proxy operator[](int i) const
    {
        if (i >= rows) throw std::out_of_range("");
        return Proxy(mtx + cols * i, cols);
    }
    Matrix& operator*=(int x)
    {
        for (int i = 0; i < cols * rows; i++)
            mtx[i] *= x;
        return *this;
    }
    bool operator==(const Matrix& other) const
    {
        if (this == &other)
            return true;
        if (cols != other.cols || rows != other.rows)
            return false;
        for (int i = 0; i < cols * rows; i++)
            if (mtx[i] != other.mtx[i])
                return false;
        return true;
    }
    bool operator!=(const Matrix& other) const
    {
        return !(*this == other);
    }

};
