#include <iostream>
#include <cmath>
#include "numbers.dat"


bool IsSimple(const int x) {
    for (int i = 2; i <= sqrt(x); ++i) {
        if (x % i == 0) {
            return false;
        }
    }
    if (x == 1)   return false;
    return true;
}


int main(int argc, char* argv[]) {
    if (argc == 1 || argc % 2 == 0) {
        return -1;
    }

    for (size_t i = 1; i < argc; ++i) {
          int x = std::atoi(argv[i]);
          i++;
          int y = std::atoi(argv[i]);
          if (x > y) {
              std::cout << 0 << '\n';
              return 0;
          }

          int count = 0, left = 0, right = Size - 1;
          while (left < Size && Data[left] < x)   left++;
          if (Data[left] > x || left == Size)  return -1;

          while (right >= 0 && Data[right] > y)   right--;
          if (Data[right] != y || right == -1)  return -1;

          while (left < right) {
              if (IsSimple(Data[left]))   count++;
              left++;
          }
          std::cout << count << std::endl;
    }

    return 0;
}
