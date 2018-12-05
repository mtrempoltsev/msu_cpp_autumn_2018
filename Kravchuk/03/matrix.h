#include <vector>
#include <stdexcept>

class Matrix {
public:
  Matrix (size_t n_rows_arg, size_t n_columns_arg) {
    n_rows = n_rows_arg;
    n_columns = n_columns_arg;
    data = new int [n_rows * n_columns];
  }

  ~Matrix () {
    delete [] data;
  }

  size_t getRows () const { return n_rows; }
  size_t getColumns () const { return n_columns; }

  Matrix& operator *= (int rvalue) {
    for (unsigned int i = 0; i < n_rows; ++i)
      for (unsigned int j = 0; j < n_columns; ++j)
        data[i * n_columns + j] *= rvalue;
  }

  bool operator == (const Matrix& rvalue) {
    if (n_rows != rvalue.n_rows || n_columns != rvalue.n_columns)
      return false;
    for (unsigned int i = 0; i < n_rows; ++i)
      for (unsigned int j = 0; j < n_columns; ++j)
        if (data[i * n_columns + j] != rvalue.data[i * rvalue.n_columns + j])
          return false;
    return true;
  }

  bool operator != (const Matrix& rvalue) {
    return !(*this == rvalue);
  }

  class dummy {
    int *data;
    size_t n_columns;
  public:
    dummy (int *data_arg, size_t n_columns_arg):
      data (data_arg),
      n_columns (n_columns_arg)
    {}

    int& operator [] (size_t index) {
      if (index >= n_columns)
        throw std::out_of_range ("");
      return data[index];
    }
    const int& operator [] (size_t index) const {
      if (index >= n_columns)
        throw std::out_of_range ("");
      return data[index];
    }
  };

  const dummy operator [] (size_t index) const {
    if (index >= n_rows)
      throw std::out_of_range ("");
    return dummy (data + index * n_columns, n_columns);
  }

  dummy operator [] (size_t index) {
    if (index >= n_rows)
      throw std::out_of_range ("");
    return dummy (data + index * n_columns, n_columns);
  }


public:
  int *data;
  size_t n_rows = 0;
  size_t n_columns = 0;
};
