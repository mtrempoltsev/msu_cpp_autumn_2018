#include <iostream>
#include "numbers.dat"

using std::cout;
using std::endl;

struct indices {
    int first_index = -1;
    int last_index = -1;
};

bool check_simplicity(int n)
{
    if (n == 1)
        return false;
    
    for (int i = 2; i * i <= n; i++)
    {
        if (!(n % i))
            return false;
    }
    return true;
}

int left_linear_search(const int* Data, int Size, int key)
{
    for (int i = 0; i < Size; i++)
    {
        if (Data[i] == key)
            return i;
    }
    return -1;
}

int right_linear_search(const int* Data, int Size, int key)
{
    for (int i = Size - 1; i > 0; i--)
    {
        if (Data[i] == key)
            return i;
    }
    return -1;
}


indices indices_in_array(const int* Data, int Size, int first_number, int last_number)
{
    indices tmp;
    
    if (first_number > last_number)
        return tmp;

    tmp.first_index = left_linear_search(Data, Size, first_number);
    tmp.last_index = right_linear_search(Data, Size, last_number);

    return tmp;
}

int simple_in_range(const int* Data, int Size, int first_number, int last_number)
{
    indices tmp = indices_in_array(Data, Size, first_number, last_number);
    int count = 0;

    if (tmp.first_index == -1 || tmp.last_index == -1)
        return 0;

    for (int i = tmp.first_index; i <= tmp.last_index; i++)
    {
        if (check_simplicity(Data[i]))
            count++;
    }

    return count;
}

int main(int argc, char* argv[])
{
    if (!(argc % 2) || (argc == 1))
        return (-1);

    for (int i = 1; i < argc; i += 2)
    {
        int first_number = std::atoi(argv[i]);
        int last_number = std::atoi(argv[i + 1]);
        int tmp = simple_in_range(Data, Size, first_number, last_number);
        cout << tmp << endl;
    }
    return 0;
}
