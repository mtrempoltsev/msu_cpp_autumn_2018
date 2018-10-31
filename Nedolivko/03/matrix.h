typedef int elem_t;

class Matrix
{
	int rows;
	int columns;
	elem_t* matrix;
	
public:
	class Row
	{
		int size;
		elem_t *row;
	public:
		Row(int sz, elem_t *elms): size(sz), row(elms){}
		int& operator [](int i)
		{
			if (i >= size) {
				throw std::out_of_range("");
			}
			return row[i];
		}
		int operator [](int i) const
		{
			if (i >= size) {
				throw std::out_of_range("");
			}
			return row[i];
		}
	};
	
	Matrix(int rws, int clmns): rows(rws), columns(clmns), matrix(new elem_t[rws*clmns]){}
	
	int getRows()
	{
		return rows;
	}
	int getColumns()
	{
		return columns;
	}
	Row operator [] (int i)
	{
		if (i >= rows) {
			throw std::out_of_range("");
		}
		return Row(columns, &matrix[i*columns]);
	}
	const Row operator[](int i) const
	{
		if (i >= rows) {
			throw std::out_of_range("");
		}
		return Row(columns, &matrix[i*columns]);
	}
	bool operator==(const Matrix& other) const
	{
		if (this->rows == other.rows && this->columns == other.columns) {
			for (int i = 0; i < rows*columns; i++) {
				if (matrix[i] != other.matrix[i]) {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	}
	bool operator!=(const Matrix& other) const
	{
		return !(*this == other);
	}
	Matrix& operator *=(int coef)
	{
		for (int i = 0; i < rows*columns; i++) 
		{
			matrix[i] *= coef;
		}
		return *this;
	}
	~Matrix()
	{
		delete[] matrix;
	}
};
