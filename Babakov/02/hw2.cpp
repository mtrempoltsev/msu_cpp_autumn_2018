#include "numbers.dat"
#include <iostream>
#include <cmath>

size_t find_first(const int arr[], int arr_size,int num) {
    for (size_t i = 0; i < arr_size; i++) {
        if (arr[i] == num) {
            return i;
        }
    }
    return -1;
}

size_t find_last(const int arr[], int arr_size, size_t from, int num) {
    size_t desired_index = -1;
    for (size_t i = from; i < arr_size; i++) {
        if (arr[i] == num)
            desired_index = i;
    }
    return desired_index;
}

int Prime(int a) //universal prime number searcher
{
   unsigned long i, j, max_divider;
   if (a == 0 || a == 1)                // 
      return 0;                         // block of quick
   if (a == 2 || a == 3 || a == 5)      // check if 
      return 1;                         // number is prime
   if (a%2 == 0 || a%3 == 0 || a%5 == 0)// or not
      return 0;                         //
   // maximal divider of number can't be  
   // more than sqrt(number)
   max_divider = sqrt((double)a);             
   i = 7; j = 11;                        
   while (j <= max_divider && a%i && a%j)
   {
       i += 6; j += 6;
   }
   if (j <= max_divider || i <= max_divider && a%i == 0)
      return 0;
   return 1;
}

int prime_out (const int arr[], size_t from, size_t to) {
    int prime_num_amount = 0;
    for (int i = from; i <= to; i++) {
        prime_num_amount += Prime(arr[i]);
    }
    std::cout << prime_num_amount << std::endl;
    return 0;
}

int main(int argc, char* argv[])
{
    if ((argc < 2) || !(argc % 2)){
        return -1;
    }
    int pair_num = argc  / 2;
    for (size_t i = 0; i < pair_num; i++)
    {
        int start = std::atoi(argv[2*i + 1]);
        int end = std::atoi(argv[2*(i + 1)]);
        if (start > end){
            std::cout << 0 << std::endl;
            return 0;
        }
        size_t start_index = find_first(Data, Size, start);
        size_t last_index = find_last(Data, Size, start_index, end);
        if (start_index == -1 || last_index == -1)
            return -1;
        prime_out (Data, start_index, last_index);
    }
    return 0;
}