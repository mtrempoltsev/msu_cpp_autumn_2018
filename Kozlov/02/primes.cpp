#include <iostream>
#include <cstdlib>
#include "numbers.dat"

bool prime(int a);
int lower(int a);
int upper(int a);
bool check(int a, int b);


bool check(int a, int b)
{
    return (std::min(a,b) < 0 || std::max(a,b) > 100000);
}

bool prime(int a)
{
   int i;
   if (a <= 1 || a % 2 == 0)  return 0;
   for(i = 3; i * i <= a && a % i; i += 2);
   return a == 2 || i * i > a;
}

int lower(int a)
{
    int l = 0, r = Size - 1;

    while (l < r)
    {
        int m = l + (r - l) / 2;
        if (Data[m] >= a)
            r = m;
        else
            l = m + 1;
    }
    return Data[l] == a ? l : -1;
}

int upper(int a)
{
      int l = 0, r = Size - 1;
      while (l < r)
      {
        int m = r - (r - l) / 2;
        if (Data[m] <= a)
          l = m;
        else
          r = m - 1;
      }
      return Data[l] == a ? l : -1;
}

int process(int l, int r)
{
    if (check(l, r)) return 0;
    int counter = 0;
    while (l <= r)
    {
        if (prime(Data[l]))
        {
            do
            {
                counter++;
                l++;
            }
            while (l <= r && Data[l] == Data[l-1]);
        }
        else
        {
            do
            {
                l++;
            }
            while (l <= r && Data[l] == Data[l-1]);
        }
    }
    return counter;
}



int main(int argc, char **argv)
{
    int i, a_index, b_index;

    if (argc < 2 || argc % 2 == 0) return 1;

    for (i = 1; i < argc; i += 2)
    {
        a_index = lower(std::atoi(argv[i]));
        b_index = upper(std::atoi(argv[i+1]));
        std::cout << process(a_index, b_index) << std::endl;
    }
    return 0;
}
