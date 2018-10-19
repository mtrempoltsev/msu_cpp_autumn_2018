#include <iostream>
#include <math.h>
#include "numbers.dat"

using namespace std;

inline bool check_simppe (int n) {
  if (n == 1)
    return false;
  for (int dev = 2; dev <= sqrt(n); ++dev)
    if (n % dev == 0)
      return false;
  return true;
}

int main(int argc, char *argv[])
{
  // Check amount of args
  if (argc % 2 == 0 || argc == 1)
    return -1;
  for (int pair = 1; pair < argc; pair += 2) {
    int first = atoi(argv[pair]);
    int last = atoi(argv[pair + 1]);
    if (first > last) {
      cout << 0 << endl;
      continue;
    }
    int pos = 0;
    int res = 0;
    // Search position for first
    for (; Data[pos] != first && pos < Size; ++pos);
    if (pos == Size)
      return -1;
    for (; Data[pos] <= last && pos < Size; ++pos)
      if (check_simppe(Data[pos]))
        res++;
    cout << res << endl;
  }
  return 0;
}

