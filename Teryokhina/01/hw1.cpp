#include <vector>
#include <iostream>
#include "numbers.dat"

using namespace std;

void initialize_primes(vector<char>& prime_array){
  int size = prime_array.size();

  prime_array[0] = prime_array[1] = false;

  for(int i = 2; i * i <= size; ++i)
    if (prime_array[i] && i * i <= size)
      for(int j = i * i; j <= size; j += i) 
        prime_array[j] = false;
}

int find_indexes(const int from, const int to, int &start, int &end){
  int i;
  for(i = 0; i < Size; ++i)
    if(Data[i] == from){
      start = i;
      break;
    }

  if(i == Size)
    return -1;

  for(i = Size - 1; i >= start; --i)
    if(Data[i] == to){
      end = i;
      break;
    }

  if(i < start)
    return -1;
  end = i;

  return 0;
}

int main(int argc, char **argv){
  const size_t maxInt = 100000;
  vector<char> isPrime(maxInt, true);

  if(argc == 1 || argc % 2 == 0)
    return -1;

  initialize_primes(isPrime);
  
  for (int i = 1; i < argc; i += 2)
  {
      int from = std::atoi(argv[i]);
      int to   = std::atoi(argv[i + 1]);
      int fromInd, toInd, count = 0;

      int result = find_indexes(from, to, fromInd, toInd);
      
      if (result == -1){
        cout << 0 << endl;
        continue;
      }

      for(int i = fromInd; i <= toInd; ++i){
        count += isPrime[Data[i]];
      }

      cout << count << endl;
  }
  return 0;
}