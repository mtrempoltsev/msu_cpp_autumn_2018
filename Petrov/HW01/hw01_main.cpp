#include<iostream>
#include<cstdlib>
#include "numbers.dat"

using namespace std;

bool isPrime(int n)
{
    if(n < 2){
        return false;
    }
    if(n < 4){
        return true;
    }
    for(int i = 2; i * i <= n; ++i){
        if(n % i == 0){
            return false;
        }
    }
    return true;
}

int first(int value, const int *arr, size_t n)
{
    for(int i = 0; i < n && value >= arr[i]; ++i){
        if(value == arr[i]){
            return i;
        }
    }
    return -1;
}

int last(int value, int start, const int *arr, size_t n)
{
    int i = start;
    for(; i < n && value >= arr[i]; ++i){
        if(value == arr[i]){
            for(; i < n && value == arr[i]; ++i);
            return i - 1;
        }
    }
    return -1;
}

int main(int argc, char* argv[]){

    constexpr int MAX_NUM = 100000;

    if(argc % 2 == 0 || argc == 1){
        return -1;
    }
    for(size_t it = 1; it < argc; it += 2){
        int v1 = atoi(argv[it]);
        int v2 = atoi(argv[it + 1]);
        if(v1 < 0 || v2 < 0 || v1 > MAX_NUM || v2 > MAX_NUM){
            return -1;
        }
        int a = first(v1, Data, Size);
        int b = last(v2, a, Data, Size);
        int sum = 0;

        if(a == -1 || b == -1){
            cout << 0 << endl;
            continue;
        }

        for(size_t i = a; i <= b; ++i){
            if(isPrime(Data[i])){
                ++sum;
            }
        }
        cout << sum << endl;
    }
    return 0;
}
