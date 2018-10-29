#include<stdexcept>

class Matrix
{
private:
    int **_matrix;
    size_t _rows, _columns;

    class Row
    {
        int *_row;
        size_t _n;

    public:

        Row(int *row, size_t n) : _row(row), _n(n) {}
        int& operator[](size_t);
        const int& operator[] (size_t) const;
    };
public:

    Matrix(size_t, size_t);

    size_t getRows() const;
    size_t getColumns() const;

    Matrix& operator*= (int);
    bool operator== (const Matrix&) const;
    bool operator!= (const Matrix&) const;
    Row operator[] (size_t);
    const Row operator[] (size_t) const;

    ~Matrix();
};

Matrix::Matrix(size_t rows, size_t columns) : _rows(rows), _columns(columns)
{
    if(rows == 0 || columns == 0) {
        _matrix = nullptr;
    }
    else {
        _matrix = new int *[_rows];
        for(size_t i = 0; i < _rows; ++i) {
            _matrix[i] = new int[_columns];
        }
    }
}

size_t Matrix::getRows() const
{
    return _rows;
}

size_t Matrix::getColumns() const
{
    return _columns;
}

Matrix& Matrix::operator*=(int value)
{
    for(size_t i = 0; i < _rows; ++i) {
        for(size_t j = 0; j < _columns; ++j) {
            _matrix[i][j] *= value;
        }
    }

    return *this;
}

bool Matrix::operator==(const Matrix& rightMatrix) const
{
    if(_rows != rightMatrix._rows || _columns != rightMatrix._columns) {
        return false;
    }

    for(size_t i = 0; i < _rows; ++i) {
        for(size_t j = 0; j < _columns; ++j) {
            if(_matrix[i][j] != rightMatrix._matrix[i][j]) {
                return false;
            }
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix& rightMatrix) const
{
    return !(*this == rightMatrix);
}

Matrix::Row Matrix::operator[](size_t index_r)
{
    if(index_r >= _rows) {
        throw std::out_of_range("");
    }
    return Row(_matrix[index_r], _columns);
}

const Matrix::Row Matrix::operator[](size_t index_r) const
{
    if(index_r >= _rows) {
        throw std::out_of_range("");
    }
    return Row(_matrix[index_r], _columns);
}

int& Matrix::Row::operator[](size_t index_c)
{
    if(index_c >= _n) {
        throw std::out_of_range("");
    }
    return _row[index_c];
}

const int& Matrix::Row::operator[](size_t index_c) const
{
    if(index_c >= _n) {
        throw std::out_of_range("");
    }
    return _row[index_c];
}

Matrix::~Matrix()
{
    if(_rows != 0 && _columns != 0) {
        for(size_t i = 0; i < _rows; ++i) {
            delete[] _matrix[i];
        }
        delete[] _matrix;
    }
}
