#pragma once

class Matrix {
private:
    size_t NCols;
    size_t NRows;
    int* Data;

    class MatrixRow {
    private:
        size_t Length;
        int* Data;

    public:
        MatrixRow(int* data, size_t length)
            : Length(length)
            , Data(data)
        {
        }

        int& operator[](size_t i) {
            if (i >= Length) {
                throw std::out_of_range("Wrong column index!");
            }
            return Data[i];
        }

        int operator[](size_t i) const {
            if (i >= Length) {
                throw std::out_of_range("Wrong column index!");
            }
            return Data[i];
        }
    };

public:
    Matrix(size_t nRows, size_t nCols)
        : NRows(nRows)
        , NCols(nCols)
        , Data(new int[NRows * NCols])
    {
    }

    ~Matrix() {
        delete[] Data;
    }

    size_t getRows() const {
        return NRows;
    }

    size_t getColumns() const {
        return NCols;
    }

    MatrixRow operator[](size_t i) {
        if (i >= NRows) {
            throw std::out_of_range("Wrong row index!");
        }
        return MatrixRow(Data + i * NCols, NCols);
    }

    const MatrixRow operator[](size_t i) const {
        if (i >= NRows) {
            throw std::out_of_range("Wrong row index!");
        }
        return MatrixRow(Data + i * NCols, NCols);
    }

    bool operator==(const Matrix& other) const {
        if (other.NCols != NCols || other.NRows != NRows) {
            return false;
        }
        for (size_t i = 0; i < NRows * NCols; ++i) {
            if (Data[i] != other.Data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    Matrix& operator*=(int factor) {
        for (size_t i = 0; i < NRows * NCols; ++i) {
            Data[i] *= factor;
        }
        return *this;
    }
};
