#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
public:
	class Proxy {
	public:
		Proxy(int* matrix_content, const size_t cols, const size_t position):
			p_content(matrix_content + cols * position),
			current_size(cols) {}
		int& operator [](const size_t position) const {
			if((current_size <= position) || (position < 0))
				throw std::out_of_range("");
			return *(p_content + position);
		}
	private:
		int* p_content;
		const size_t current_size;
	};
	Matrix(const size_t rows, const size_t cols):
		content(new int[rows * cols]),
		current_rows(rows),
		current_cols(cols) { }
	size_t getRows() const { return current_rows; }
	size_t getColumns() const { return current_cols; }
	Proxy operator [](const size_t position) const {
		if((current_rows <= position) || (position < 0))
				throw std::out_of_range("");
			return Proxy(content, current_cols, position);
	}
	bool operator !=(const Matrix& second) const {
		if(!((current_rows == second.getRows()) &&
			 (current_cols == second.getColumns()))) return true;
		for(size_t i = 0; i < current_rows; i++)
			for(size_t j = 0; j < current_cols; j++)
				if((*this)[i][j] == second[i][j]) return false;
		return true;
	}
	bool operator ==(const Matrix& second) const { return !(second != *this); }
	Matrix& operator *=(int multiplier) {
		for(size_t i = 0; i < current_rows; i++)
			for(size_t j = 0; j < current_cols; j++)
				(*this)[i][j] *= multiplier;
		return *this;
	}
	~Matrix() { delete[] content; }
private:
	int* content;
	const size_t current_rows, current_cols;
};

#endif