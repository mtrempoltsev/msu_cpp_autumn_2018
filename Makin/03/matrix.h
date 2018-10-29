class Matrix
{
private:
	size_t rows, columns;	
	int* data;

public:
	class Row
	{
	private:
		size_t size;
		int* data;
	public:
		Row(int* data, size_t size) : data(data), size(size) {};
		int& operator [] (size_t j)
		{
			if (j >= size)
			{
				throw std::out_of_range("");
			}
			return data[j];
		}

		const int &operator[](size_t j) const
		{
			if (j >= size)
			{
				throw std::out_of_range("");
			}
			return data[j];
		}

	};

	Matrix(size_t rows, size_t columns) : rows(rows), columns(columns)
	{
		if ((rows < 0) || (columns < 0))
		{
			throw std::out_of_range("");
		}

		data = new int[rows*columns];
	}

	~Matrix()
	{
		delete[] data;
	}

	Row operator [] (size_t pos)
	{
		if (pos >= rows)
		{
			throw std::out_of_range("");
		}
		return Row(&(data[pos * columns]), columns);
	}

	const Row operator[](size_t pos) const
	{
		if (pos >= rows)
		{
			throw std::out_of_range("");
		}
		return Row(&(data[pos * columns]), columns);
	}

	Matrix& operator *= (int num)
	{
		for (size_t i = 0; i < rows*columns; i++) data[i] *= num;
		return *this;
	}

	bool operator == (const Matrix& other) const
	{
		if (rows != other.rows)
		{
			return false;
		}
		if (columns != other.columns)
		{
			return false;
		}

		for (size_t i = 0; i < rows*columns; i++)
		{
			if (data[i] != other.data[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator != (const Matrix& other) const
	{
		return !(*this == other);
	}

	int getColumns()
	{
		return columns;
	}

	int getRows()
	{
		return rows;
	}
};