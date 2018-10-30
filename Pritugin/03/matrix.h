#include <cstring>

class Matrix
{
	size_t rows, columns;
	size_t size;
	int* data;

public:
	class Row
	{
	private:
		int* data;
		size_t size;
	public:
		Row(int* data, size_t size) : data(data), size(size) {};
		
		int operator [] (size_t pos) const
		{
			if (pos < size)
				return data[pos];
			throw std::out_of_range("");
		}
		
		int& operator [] (size_t pos)
		{
			if (pos < size)
				return data[pos];
			throw std::out_of_range("");
		}
	};
	
	Matrix(size_t rows = 0, size_t columns = 0) : rows(rows), columns(columns), size(rows * columns)
	{
		data = new int[size];
	}
	
	~Matrix()
	{
		delete[] data;
	}
	
	Row operator [] (size_t pos) const
	{
		if (pos < rows)
			return Row(&(data[pos * columns]), columns);
		throw std::out_of_range("");
	}
	
	Row operator [] (size_t pos)
	{
		if (pos < rows)
			return Row(&(data[pos * columns]), columns);
		throw std::out_of_range("");
	}

	Matrix& operator *= (int num)
	{
		for (size_t i = 0; i < size; ++i) data[i] *= num;
		return *this;
	}
	
	bool operator == (const Matrix& other) const
	{
		if (rows != other.rows || columns != other.columns)
			return false;
		if(memcmp(data, other.data, size) == 0)
			return true;
		return false;
	}

	bool operator != (const Matrix& other) const
	{
		return !(*this == other);
	}
	size_t getRows () 
	{ 
		return rows; 
	}
	size_t getColumns ()
	{ 
		return columns; 
	}
};


