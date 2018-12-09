#include <iostream>
#include <cstddef>

class Matrix {
public:
	Matrix() : _rows(0), _cols(0), _data(NULL) {}

	Matrix(size_t const rows, size_t const cols) : _rows(rows), _cols(cols) {
		_data = new int [_rows * _cols];
	}

	~Matrix() {delete [] _data;}

	size_t 	getRows() const {
		return _rows;
	}

	size_t getColumns() const {
		return _cols;
	}

	Matrix & operator*=(int value) {
		for (size_t i = 0; i < _rows * _cols; i++)
			_data[i] *= value;
		
		return *this;
	}

	bool operator==(Matrix const & other) const {
		if (this == &other)
			return true;

		size_t otherRows = other._rows;
		size_t otherCols = other._cols;

		if (otherRows != _rows || otherCols != _cols)
			return false;

		bool b = true;

		for (size_t i = 0; i < _rows * _cols; i++) {
			if (other._data[i] != _data[i]) {
				b = false;
				break;
			}
		}

		return b;
	}

	bool operator!=(Matrix const & other) const {
		return !(*this == other);
	}

	class TmpClass {
	public:
		TmpClass() {}

		TmpClass(int * data_tmp, size_t cols_tmp)
			: _data_tmp(data_tmp), _cols_tmp(cols_tmp) {}

		~TmpClass() {}

		int const & operator[](size_t index) const {
			if (index >= _cols_tmp)
				throw std::out_of_range("");

			return *(_data_tmp + index);
		}

		int & operator[](size_t index) {
			if (index >= _cols_tmp)
				throw std::out_of_range("");

			return *(_data_tmp + index);
		}

	private:
		int * _data_tmp;
		size_t _cols_tmp;
	};

	TmpClass const operator[](size_t index) const {
		if (index >= _rows)
			throw std::out_of_range("");

		return TmpClass(_data + index * _cols, _cols);
	}

	TmpClass operator[](size_t index) {
		if (index >= _rows)
			throw std::out_of_range("");

		return TmpClass(_data + index * _cols, _cols);
	}

private:
	size_t const _rows;
	size_t const _cols;
	int * _data;
};