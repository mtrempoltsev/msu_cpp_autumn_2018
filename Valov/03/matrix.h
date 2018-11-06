class Matrix
{
	int* Mat;
	const int Size, Rows, Columns;
	class Matrix_Row
	{
		int size_R;
		int* Mat_R; 
	public:
		Matrix_Row(int* a, int b) : Mat_R(a), size_R(b)
		{}
		const int& operator [] (int index_row) const
		{
			if (index_row >= size_R)
				throw std::out_of_range("");
			return Mat_R[index_row];
		}
		int& operator [] (int index_row)
		{
			if (index_row >= size_R)
				throw std::out_of_range("");
			return Mat_R[index_row];
		}
	};
public:
	Matrix(int str, int col) : Rows(str), Columns(col), Size(str * col)
	{
		Mat = new int [Size];
	}
	const Matrix_Row operator [] (int index_row) const
	{
		if(index_row >= Rows)
			throw std::out_of_range("");
		return Matrix_Row(Mat + index_row * Columns, Columns);
	}
	Matrix_Row operator [] (int index_row)
	{
		if(index_row >= Rows)
			throw std::out_of_range("");
	 	return Matrix_Row(Mat + index_row * Columns, Columns);
	}
        Matrix& operator *= (int Mul)
        {
	        for (int i = 0; i < Size; ++i)
			Mat[i] *= Mul;
		return *this;
 	}
        bool operator == (const Matrix& other)
        {
	        if ((Rows != other.getRows()) || (Columns != other.getColumns())) 
	        	return 0;
		for (int i = 0; i < Size; ++i)
		{
			if (Mat[i] != other.Mat[i])
				return 0;
		}
		return 1;
	}
	bool operator != (const Matrix& other)
	{
		return !(*this == other);
	}
	int getRows() const
	{
		return Rows;
	}
	int getColumns() const
	{
		return Columns;
	}
	~Matrix()
	{
		delete[] Mat;
	}
};
