#include <iostream>

class Matrix
{
	int** array;
	size_t rows;
	size_t columns;
	
	class MatrixTemp
	{
		Matrix* matrix;
		const Matrix* matrixConst;
		size_t i;
		
	public:
		MatrixTemp(const size_t i, Matrix* matrix)
			: i(i), matrix(matrix) {}
		
		MatrixTemp(const size_t i, const Matrix* matrix)
			: i(i), matrixConst(matrix) {}
		
		int& operator [] (const size_t j)
		{
			if (j >= matrix->columns)		
			{
				throw std::out_of_range("");
			}
			else
			{
				return matrix->array[i][j];	
			}
		}
		
		int operator [] (const size_t j) const
		{
			if (j >= matrixConst->columns)		
			{
				throw std::out_of_range("");
			}
			else
			{
				return matrixConst->array[i][j];	
			}
		}
	};

public:	
	Matrix(size_t rows = 0, size_t columns = 0)
		: rows(rows), columns(columns)
    {
		array = new int*[rows];
		for (size_t i = 0; i < rows; ++i)
		{
			array[i] = new int[columns];
		}
    }
	
	~Matrix()
	{
		for (size_t i = 0; i < rows; ++i)
		{
			delete [] array[i];
		}
		delete [] array;
	}
	
	size_t getRows() const
	{
		return rows;
	}
	
	size_t getColumns() const
	{
		return columns;
	}
	
	Matrix& operator *= (const int num)
	{
		Matrix tmp(rows, columns);
		
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < columns; ++j)
			{
				array[i][j] *= num;
			}
		}
		
		return *this;
	}
	
	bool operator == (const Matrix& other)
	{
		if ((this->rows != other.getRows()) ||
		    (this->columns != other.getColumns()))
		{
			return false;
		}
		
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < columns; ++j)
			{
				if (this -> array[i][j] != other.array[i][j])
				{
					return false;
				}
			}
		}
		
		return true;
	}
	
	bool operator != (const Matrix& other)
	{
		if ((this->rows != other.getRows()) ||
		    (this->columns != other.getColumns()))
		{
			return true;
		}
		
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < columns; ++j)
			{
				if (this->array[i][j] != other.array[i][j])
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	MatrixTemp operator [] (const size_t i)
	{
		if (i >= rows)
		{
			throw std::out_of_range("");
		}
		else
		{
			return MatrixTemp(i, this);		
		}
	}
	
	const MatrixTemp operator [] (const size_t i) const
	{
		if (i >= rows)
		{
			throw std::out_of_range("");
		}
		else
		{
			return MatrixTemp(i, this);		
		}
	}
};
	
