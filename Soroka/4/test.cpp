#include <iostream>

#include "matrix.h"


void equalCheck(int a, int b, int i, int j)
{
    if (a != b)
        std::cout << "Expected " << b << " got " << a << " in (" << i << "," << j << ")" << std::endl;
}

int main()
{
    int n = 0;

    Matrix m1(2, 3);
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            m1[row][col] = ++n;
        }
    }

    equalCheck(m1[0][0], 1, 0, 0);
    equalCheck(m1[0][1], 2, 0, 1);
    equalCheck(m1[0][2], 3, 0, 2);
    equalCheck(m1[1][0], 4, 1, 0);
    equalCheck(m1[1][1], 5, 1, 1);
    equalCheck(m1[1][2], 6, 1, 2);

    if(m1.getRows() !=  2) std::cout << "Wrong row number" << std::endl;
    if(m1.getColumns() != 3) std::cout << "Wrong column number" << std::endl;



    const Matrix& m2 = m1;

    m1 *= 2;

    equalCheck(m2[0][0], 1 * 2, 0, 0);
    equalCheck(m2[0][1], 2 * 2, 0, 1);
    equalCheck(m2[0][2], 3 * 2, 0, 2);
    equalCheck(m2[1][0], 4 * 2, 1, 0);
    equalCheck(m2[1][1], 5 * 2, 1, 1);
    equalCheck(m2[1][2], 6 * 2, 1, 2);

    try
    {
        m2[3][0];
        std::cout << "Error no throws" << std::endl;
    }
    catch (const std::out_of_range&)
    {

    }
    catch (...)
    {
        std::cout << "Error in throws" << std::endl;
    }


    try
    {
        m2[0][4];
        std::cout << "Error no throws" << std::endl;
    }
    catch (const std::out_of_range&)
    {

    }
    catch (...)
    {
        std::cout << "Error in throws" << std::endl;
    }


    const Matrix& m3 = m1;

    Matrix m4(2, 3);

    if (!(m1 == m1))
        std::cout << "Error in == " << std::endl;
    if (!(m1 == m3))
        std::cout << "Error in == " << std::endl;
    if (!(m1 != m4))
        std::cout << "Error in != " << std::endl;


    std::cout << "done" << std::endl;

    return 0;
}
