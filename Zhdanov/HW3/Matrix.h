#include <cstdlib>
#include <iostream>

using namespace std;

class Matrix
{
	int rows, columns, size;
	int* data;

	class Row
	{
		int* data;
		int columns;
	public:
		Row(int* row, int columns)
			:data(row), columns(columns)
		{};
		int& operator[](int column)
		{
			if (column < columns)
				return data[column];
			else
				throw out_of_range("");
		};
		const int operator[](int column) const
		{
			if (column < columns)
				return data[column];
			else
				throw out_of_range("");
		};
	};

public:
	Matrix(int rows, int columns)
		:rows(rows), columns(columns), size(rows*columns)
	{
		data = new int[size];
	};
	~Matrix()
	{
		delete[] data;
	};

	Row operator[](int row)
	{
		if (row < rows)
			return Row(&data[row*columns], columns);
		else
			throw out_of_range("");
	};
	const Row operator[](int row) const
	{
		if (row < rows)
			return Row(&data[row*columns], columns);
		else
			throw out_of_range("");
	};
	int getRows()
	{
		return rows;
	};

	int getColumns()
	{
		return columns;
	};

	bool operator == (const Matrix& matrix) const
	{
		if (rows == matrix.rows && columns == matrix.columns)
		{
			if (memcmp(data, matrix.data, size) == 0)
				return true;
		};
		return false;
	};

	bool operator != (const Matrix& matrix) const
	{
		return !(*this == matrix);
	};

	Matrix& operator *= (int num)
	{
		for (int i = 0; i < size; ++i)
		{
			data[i] *= num;
		};
		return *this;
	};
};