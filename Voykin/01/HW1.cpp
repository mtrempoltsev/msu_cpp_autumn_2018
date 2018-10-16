#include <iostream>
#include <cmath>
#include "numbers.dat"
using namespace std;

int search_limit(int x)
{
    int left_limit = 0, right_limit = Size - 1;
    int cursor;
    while(1)
    {
        cursor = (left_limit + right_limit)/2;
        if (Data[cursor] != x)
        {
            if (Data[cursor] < x)
            {
                left_limit = cursor;
            }
            else
            {
                right_limit = cursor;
            }
        }
        else
        {
            while ((cursor > 0) && (Data[cursor] == Data[cursor-1]))
            {
                cursor--;
            }
            return cursor;
        }
        if (right_limit - left_limit < 2)
        {
            if (Data[right_limit] == x)
            {
                return right_limit;
            }
            else
            {
                return -1;
            }
        }
    }
}
void it_no_simple(bool* praim, int n)
{
    praim[0] = praim[1] = true;
    for (int i=2; i*i < n; i++)
    {
        if (!praim[i])
        {
            for (int j = i*i; j < n; j += i)
            {
                praim[j] = true;
            }
        }
    }
}
void search_prime_numbers(int left_limit, int stop,bool *praim)
{
    int number_of_simple = 0;
    int i;
    for (i = left_limit; (i < Size) && (Data[i] <= stop); i++)
    {
        number_of_simple += !praim[Data[i]];
    }
    if (Data[i-1] == stop)
    {
        cout << number_of_simple << endl;
    }
    else
    {
        cout << 0 << endl;
    }
}

int main(int argc, char* argv[])
{
    int left_limit;
    bool *praim = new bool [Data[Size-1]+1]{};
    if (argc == 1)
    {
        cout << 0 << endl;
        return -1;
    }
    if (argc % 2 == 0)
    {
        return -1;
    }
    for (int i = 1; i < argc; i+=2)
    {
        left_limit = search_limit(atoi(argv[i]));
        if (left_limit != -1)
        {
            it_no_simple(praim,Data[Size-1]+1);
            search_prime_numbers(left_limit, atoi(argv[i+1]), praim);
        }
        else
        {
            cout << 0 << endl;
        }
    }
    delete []praim;
    return 0;
}
