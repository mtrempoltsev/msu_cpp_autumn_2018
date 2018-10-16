#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include "numbers.dat"


using std::pair;
using std::make_pair;
using std::vector;


const int max_numbers = 100000;


void eratosfen(vector<bool>&);
int process(pair<int, int>, const vector<int>&, const vector<bool>&);
int lower_bound(int);
int upper_bound(int);


int main(int argc, char* argv[]) {
    vector<bool> primes(max_numbers + 1, true);
    vector<pair<int, int>> borders;
    vector<int> data(Data, Data + Size);
    
    
    if ((argc - 1) % 2 || argc == 1) {
        std::cout << -1 << std::endl;
        return -1;
    } else {
        for (int i = 1; i < argc; i += 2) {
            borders.push_back(make_pair(atoi(argv[i]), atoi(argv[i + 1])));
        }
    }
    eratosfen(primes);
    

    for (int i = 0; i < borders.size(); i++) {
        std::cout << process(borders[i], data, primes) << std::endl;
    }
    return 0;
}


int lower_bound(int key) {
    int mid = Size / 2;
    int left = 0;
    int right = Size - 1;
    if (key < Data[left])
        return -1;
    if (key == Data[left])
        return left;
    while (right - left > 1) {
        if (Data[mid] >= key) {
            right = mid;
        } else {
            left = mid;
        }
            mid = (right + left) / 2;
    }
    return right;
}

int upper_bound(int key) {
    int mid = Size / 2;
    int left = 0;    
    int right = Size - 1;
    if (key > Data[right]) {
        return -1;
    }
    if (key == Data[right]) {
        return right;
    }
    while (right - left > 1) {
        if (Data[mid] <= key) {
            left = mid;
        } else {
            right = mid;
        }
        mid = (right + left) / 2;
    }

    return left;
}


int process(pair<int, int> borders, const vector<int>& data, const vector<bool>& primes) {
    int left = lower_bound(borders.first);
    int right = upper_bound(borders.second);
    if (Data[left] != borders.first || Data[right] != borders.second) {
        return -1;
    }


    int counter = 0;
    for (int i = left; i <= right; i++) {
        if (primes[Data[i]]) {
            counter++;
        }
    }
    return counter;
}


void eratosfen(vector<bool>& primes) {
    primes[0] = primes[1] = false;
    for (int i = 2; i * i <= max_numbers; i++) {
        if (primes[i]) {
            for (int j = i * i; j <= max_numbers; j += i) {
                primes[j] = false;
            }
        }
    }
}
