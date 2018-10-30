#pragma once

#include <iostream>
#include <cstring>


class Matrix
{
public:
	Matrix(const size_t _rows, const size_t _columns)
		: rows(_rows)
		, columns(_columns)
	{
		matrix = new int[rows * columns];
	}

	~Matrix()
	{
		delete[] matrix;
	}

	class Row
	{
	public:
		Row(int *_row_ptr,const size_t _cols)
			: row_ptr(_row_ptr)
			, cols(_cols)
		{};

		~Row() {};

		int operator[](int j) const
		{
			if (j < 0 || j >= cols) {
				throw std::out_of_range("");
			}

			return row_ptr[j];
		}

		int& operator[](int j)
		{
			if (j < 0 || j >= cols) {
				throw std::out_of_range("");
			}

			return row_ptr[j];
		}
	private:
		int *row_ptr;
		const size_t cols;
	};

	size_t getRows() const
	{
		return this->rows;
	}

	size_t getColumns() const
	{
		return this->columns;
	}

	const Row operator[](int i) const
	{
		if (i < 0 || i >= rows) {
				throw std::out_of_range("");
		}

		return Row(matrix + i * columns, columns);
	}

	Row operator[](int i)
	{
		if (i < 0 || i >= rows) {
				throw std::out_of_range("");
		}

		return Row(matrix + i * columns, columns);
	}

	Matrix& operator*=(const int number)
	{
		for (size_t i = 0; i < rows * columns; i++) {
			matrix[i] *= number;
		}

		return *this;
	}

	bool operator==(const Matrix& other) const
	{
		if (this == &other) {
			return true;
		}

		return (rows == other.rows &&
				columns == other.columns &&
				memcmp(matrix, other.matrix, rows * columns * sizeof(int)) == 0);
	}

	bool operator!=(const Matrix& other) const
	{
		return !(*this == other);
	}

private:
	const size_t rows;
	const size_t columns;
	int *matrix;
};
