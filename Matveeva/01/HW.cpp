#include<iostream>
#include<cmath>
#include "numbers.dat"
using namespace std;

void sieve(bool ne_primes[], int pr_size){
    const int m = sqrt(pr_size);
    ne_primes[1] = true;
    for(int i = 2; i <= m; i++)
    {
        for(int j = i*i; j <= pr_size; j+=i)
        {
            ne_primes[j] = true;
        }
    }
}

int bin_search(const int & a)
{
    int start = 0;
    int stop = Size-1;
    if (Data[start] != a)
    {
        int position;
        while(1)
        {
            if ((stop-start) <= 1)
                return -1;
            position = (start + stop)/2;
            if (Data[position] == a)
            {
                while (Data[position] == Data[position-1])
                {
                    position--;
                }
                return position;
            }
            if (Data[position] < a)
                start = position;
            else
                stop = position;
        }
    }
    else
        return 0;
}

int number_primes(const int & a, const int & b, bool ne_primes[])
{
    int i = bin_search(a);
    if (i == -1)
        return 0;
    int number = 0;
    for(; i < Size && Data[i] <= b; i++)
    {
       if (ne_primes[Data[i]] == 0)
            number++;
    }
    if (Data[i-1] < b)
        return 0;
    return number;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << 0 << endl;
        return -1;
    }
    if (argc%2 == 0)
        return -1;
    int u, v;
    bool *ne_primes = new bool[Data[Size-1]+1]();
    sieve(ne_primes, Data[Size-1]);
    for (int i = 1; i < argc; i+=2)
    {
        u = atoi(argv[i]);
        v = atoi(argv[i+1]);
        if (u <= v)
            cout << number_primes(u, v, ne_primes) << endl;
        else
            cout << 0 << endl;
    }
    delete[] ne_primes;
    return 0;
}
