#include "numbers.dat"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<short> create_vector_of_primes()
{
    size_t vectorSize = 1000000;
    vector<short> prime (vectorSize + 1, 1);
    prime[0] = prime[1] = 0;
    for (int i = 2; i * i <= vectorSize; ++i)
        if (prime[i])
            if (i * i <= vectorSize)
                for (int j = i * i; j <= vectorSize; j += i)
                    prime[j] = 0;
    return prime;
}


int func(const vector<short>& prime, int first, int last)
{
    if(first > last) return 0;
    
    auto left = lower_bound(Data, Data + Size, first);
    auto right = upper_bound(Data, Data + Size, last);
    
    if((*(right - 1) != last) || (*left != first)) return 0;
    
    int count = 0;

    while(left < right){
        count += prime[*left];
        ++left;
    }
    return count;
}


int main(int argc, char* argv[])
{
    vector<short> prime = create_vector_of_primes();
    
    if((argc <= 1) || (argc % 2 == 0)) return -1;
    
    for (int i = 1; i < argc; i += 2) cout<< func(prime, atoi(argv[i]), atoi(argv[i+1])) << endl;

    return 0;
}
