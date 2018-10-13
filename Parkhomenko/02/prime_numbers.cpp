#include <iostream>
#include "stdlib.h"
#include <cmath>
#include "numbers.dat"

using namespace std;

int main(int argc, char* argv[])
{
    if ((argc%2 == 0) || (argc == 1))
    {
        return -1;
    }
    
    for (int k = 0; 2 * k + 2 < argc; k++)
    {
        int left = std::atoi(argv[1 + k * 2]);
        int right = std::atoi(argv[2 + k * 2]);
        int n_mat = 0;
        int current = -1;
        int last = -1;
        int flag = 0;
        for (int i = 0; i < Size; i++)
        {
            if (Data[i] == left)
            {
                current = i;
                break;
            }    
            else
            {
                if (Data[i] > left)
                {
                    std::cout << '0';
                    return 0;
                }
            }
        }
        for (int i = current; i < Size; i++)
        {
            if (Data[i] > right)
            {
                if (last == right)
                {
                    break;
                }
                else
                {
                    std::cout << '0';
                    return 0;
                }
            }
            if (Data[i] == last)
            {
                if (flag)
                {
                    n_mat += 1;   
                }
                else
                {
                    continue;
                }
            }
            else
            {
                last = Data[i];
                flag = 1;
                if (Data[i] == 1)
                {
                    flag = 0;
                    continue;
                }
                for (int j = 2; j <= sqrt(Data[i]); j++)
                {
                    if (Data[i]%j == 0)
                    {
                        flag = 0;
                        break;
                    }
                }
                if (flag)
                {
                    n_mat += 1;
                }
            }
        }  
        std::cout << n_mat << endl;  
    }
    
    return 0;
}
