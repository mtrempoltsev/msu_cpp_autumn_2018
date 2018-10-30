#include<stdexcept>

class Matrix
{
private:
	int size_cols;
	int size_rows;
	int **matrix;
	class Proxy
	{
		int *row;
		int length;
	public:
		Proxy(int *r, int cols) : row(r), length(cols) {}
		const int& operator[](int i) const
		{
			if (i < length) {
				return row[i];
			}
			throw std::out_of_range("");
		}

		int& operator[](int i) 
		{
			if (i < length) {
				return row[i];
			}
			throw std::out_of_range("");
		}
	};

public:
	Matrix(int num_rows, int num_cols) : size_cols(num_cols), size_rows(num_rows)
	{
		matrix = new int *[size_rows];
		for (int i = 0; i < size_rows; i++)
			matrix[i] = new int[size_cols];

	}
	~Matrix()
	{
		for (size_t i = 0; i < size_rows; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
	int getRows() const
	{
		return size_rows;
	}
	int getColumns() const
	{
		return size_cols;
	}
	bool operator==(const Matrix& m) const
	{
		if (this == &m) {
			return true;
		}
		if (m.size_rows != size_rows || m.size_cols != size_cols) {
			return false;
		}
		for (int i = 0; i < size_rows; ++i) {
			for (int j = 0; j < size_cols; ++j) {
				if (m.matrix[i][j] != matrix[i][j])
					return false;
			}
		}
		return true;
	}

	bool operator!=(const Matrix& m) const
	{
		return !(*this == m);
	}

	Matrix& operator*=(int num) 
	{
		for (int i = 0; i < size_rows; ++i) {
			for (int j = 0; j < size_cols; ++j) {
				matrix[i][j] *= num;
			}
		}
		return *this;
	}

	const Proxy operator[](int i) const
	{
		if (i < size_rows) {
			Proxy row(matrix[i], size_cols);
			return row;
		}
		throw std::out_of_range("");
	}

	Proxy operator[](int i)
	{
		if (i < size_rows) {
			Proxy row(matrix[i], size_cols);
			return row;
		}
		throw std::out_of_range("");
	}

};
