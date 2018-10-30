class Matrix {
private:
	int *data;
	size_t rows;
	size_t columns;
public:
	class Row {
	private:
		int *data;
		size_t cols;
	public:
		Row(int *dt, size_t size)
			: data(0)
			, cols(0) {
			data = dt;
			cols = size;
			};
		const int& operator[](size_t index) const {
			if ((index >= cols) || (index < 0))
				throw std::out_of_range("");
			return data[index];
		}
		int& operator[](size_t index) {
			if ((index >= cols) || (index < 0))
				throw std::out_of_range("");
			return data[index];
		}
	};
	Matrix(size_t rows_number, size_t columns_number)
		: rows(0)
		, columns(0) {
		data = new int[rows * columns];
		rows = rows_number;
		columns = columns_number;
	}
	~Matrix() {delete[] data;}
	const Row operator[](size_t index) const {
		if ((index >= rows) || (index < 0))
			throw std::out_of_range("");
		return Row(data + index * columns, columns);
	}
	Row operator[](size_t index) {
		if ((index >= rows) || (index < 0))
			throw std::out_of_range("");
		return Row(data + index * columns, columns);
	}
	Matrix& operator*=(int number) {
		size_t size = rows * columns;
		for(size_t index = 0; index < size; index++)
			data[index] *= number;
		return *this;
	}
	bool operator==(const Matrix& other) const {
		if (this == &other)
			return true;
		if ((rows != other.rows) || (columns != other.columns))
			return false;
		for(size_t index = 0; index < (rows * columns); index++)
			if (data[index] != other.data[index])
				return false;
		return true;
	}
	bool operator!=(const Matrix& other) const {
		return !(*this == other);
	}
	size_t getRows() const {return rows;}
	size_t getColumns() const {return columns;}
};
