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
        {
            Length = length;
            Data = data;
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
    Matrix(size_t nrows, size_t ncols)
    {
        NRows = nrows;
        NCols = ncols;
        Data = new int[NRows * NCols];
    }

    ~Matrix()
    {
        delete[] Data;
    }

    size_t getRows() const
    {
        return NRows;
    }

    size_t getColumns() const
    {
        return NCols;
    }

    bool operator==(const Matrix& matr) const
    {
        if (matr.NCols != NCols || matr.NRows != NRows) {
            return false;
        }
        for (size_t i = 0; i < NRows * NCols; ++i) {
            if (Data[i] != matr.Data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& matr) const {
        return !(*this == matr);
    }

    Matrix& operator*=(int mult) {
        for (size_t i = 0; i < NRows * NCols; ++i) {
            Data[i] *= mult;
        }
        return *this;
    }

    MatrixRow operator[](size_t i)
    {
        if (i >= NRows) {
            throw std::out_of_range("Wrong row index");
        }
        return MatrixRow(Data + i * NCols, NCols);
    }

    const MatrixRow operator[](size_t i) const
    {
        if (i >= NRows) {
            throw std::out_of_range("Wrong row index");
        }
        return MatrixRow(Data + i * NCols, NCols);
    }

};
