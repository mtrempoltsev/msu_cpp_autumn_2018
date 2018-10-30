#include <stdexcept>
#include <iostream>
using namespace std;
class Matrix
{
    class line
    {
        int *cursor_begin_line;
        size_t number_elements_in_line;
    public:
        line(int *cursor_begin_line, size_t number_elements_in_line) :
            cursor_begin_line(cursor_begin_line),
            number_elements_in_line(number_elements_in_line)
         {}

        const int operator[](size_t number) const
        {
            if (number >= number_elements_in_line)
            {
                throw out_of_range("");
            }
            return cursor_begin_line[number];
        }

        int& operator[](size_t number)
        {
            if (number >= number_elements_in_line)
            {
                throw out_of_range("");
            }
            return cursor_begin_line[number];
        }
    };
    size_t num_lines;
    size_t num_columns;
    int* matrix;
    public:
        Matrix(size_t num_lines, size_t  num_columns):
            num_lines(num_lines),
            num_columns(num_columns)
        {
            matrix = new int[num_lines * num_columns];
        }
        ~Matrix()
        {
            delete []matrix;
        }
        line operator[](int line_)
        {
            if (line_ >= num_lines)
            {
                throw out_of_range("");
            }
            return line(matrix + line_ * num_columns, num_columns);
        }
        const line operator[](int line_) const
        {
            if (line_ >= num_lines)
            {
                throw out_of_range("");
            }
            return line(matrix + line_ * num_columns, num_columns);
        }
        Matrix& operator*=(int multiplier)
        {
            size_t Size_matrix = num_lines * num_columns;
            for(size_t i = 0; i < Size_matrix; i++)
            {
                matrix[i] *= multiplier;
            }
            return *this;
        }
        const int getRows() const {
            return num_lines;
        }
        const int getColumns() const {
            return num_columns;
        }
        bool operator==(const Matrix& A) const
        {
            size_t Size_matrix = num_lines*num_columns;
            if ((A.getColumns() != num_columns) || (A.getRows() != num_lines))
            {
                return false;
            }
            for(size_t i = 0; i < Size_matrix; i++)
            {
                if (A[i / num_columns][i % num_columns] != matrix[i])
                {
                    return false;
                }
            }
            return true;
        }
        bool operator!=(const Matrix A) const
        {
            return !(*this == A);
        }
};
