#include <stdexcept>
#include <cstring>

class Matrix
{
    class Vector
    {
    public:
        Vector(int* data, size_t len): len(len), data(data)
        {
        }

        int& operator[](size_t index)
        {
            if(index < len)
                return data[index];

            throw std::out_of_range("");
        }

        int operator[](size_t index) const
        {
            if(index < len)
                return data[index];

            throw std::out_of_range("");
        }

    private:
        size_t len;
        int* data;
    };
    size_t rows;
    size_t cols;
    int* data;
public:
	Matrix(size_t rows, size_t cols): rows(rows), cols(cols)
	{
		data = new int[rows * cols];
	}

	size_t getRows() const
	{
		return rows;
	}

	size_t getColumns() const
	{
		return cols;
	}

	Matrix& operator*=(int a)
	{
		for(size_t i = 0; i < rows * cols; i++)
			data[i] *= a;

		return *this;
	}

	Vector operator[](size_t index)
	{
		if(index < rows)
			return Vector(&data[index * cols], cols);

		throw std::out_of_range("");
	}

	const Vector operator[](size_t index) const
	{
		if(index < rows)
			return Vector(&data[index * cols], cols);

		throw std::out_of_range("");
	}

	bool operator==(const Matrix& m) const
	{
		return m.rows == rows && m.cols == cols && !memcmp(data, m.data, rows * cols);
	}

	bool operator!=(const Matrix& m) const
	{
		return !(m == *this);
	}

	~Matrix()
	{
		delete[] data;
	}
};
