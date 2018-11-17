#include<iostream>
#include<cstdlib>
#include "numbers.dat"

int left_ind_search(const int Size, const int* Data, const int left_bound);

int right_ind_search(const int Size, const int* Data, const int right_bound, const int left_ind);

int prime_check(int n);

int prime_quantity(const int* Data, const int left_ind, const int right_ind);

int main(int argc, char* argv[])
{
    if ((argc == 1) || (argc % 2 == 0))
        return -1;

    for (int i = 1; i < argc; i += 2)
    {
        int left_bound = std::atoi(argv[i]);
        int right_bound = std::atoi(argv[i + 1]);

        if ((right_bound < 0) || (left_bound < 0))
            return -1;

        if (right_bound < left_bound)
        {
            std::cout << 0 << std::endl;
            continue;
        }

        int left_ind = left_ind_search(Size, Data, left_bound);

        if (left_ind == -1)
        {
            std::cout << 0 << std::endl;
            continue;
        }

        int right_ind = right_ind_search(Size, Data, right_bound, left_ind);

        if (right_ind == -1)
        {
            std::cout << 0 << std::endl;
            continue;
        }

        int quantuty = prime_quantity(Data, left_ind, right_ind);
        std::cout << quantuty << std::endl;
    }
    return 0;
}

int left_ind_search(const int Size, const int* Data, const int left_bound)
{
    int left_ind = -1;
    for (int i = 0; i < Size; i ++)
    {
        if (Data[i] == left_bound)
        {
            left_ind = i;
            break;
        }
    }
    return left_ind;
}

int right_ind_search(const int Size, const int* Data, const int right_bound, const int left_ind)
{
    int right_ind = -1;
    for (int i = left_ind; i < Size; i ++)
    {
        if ((Data[i] == right_bound) && ((i == Size - 1) || (Data[i + 1] != right_bound)))
        {
            right_ind = i;
            break;
        }
    }
    return right_ind;
}

int prime_check(int n)
{
    if ((n == 1) || (n == 0))
        return 0;
    for (int i = 2; i*i <= n; i ++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int prime_quantity(const int* Data, const int left_ind, const int right_ind)
{
    int S = 0;
    for (int i = left_ind; i <= right_ind; i++)
    {
        S += prime_check(Data[i]);
    }
    return S;
}
