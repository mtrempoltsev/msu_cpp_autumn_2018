#include<iostream>
#include<cstdlib>

#include "numbers.dat"

bool is_prime (int number) {
    if (number == 1) return 0;
    for (int i=2; i*i<=number; ++i) {
        if (number % i == 0) return 0;
    }

    return 1;
}


int main(int argc, char* argv[])
{
    if (argc == 1) return -1;
    if (argc % 2 == 0) return -1;

    int *res=new int [(argc-1)/2];
    for (int i = 1; i <= (argc-1)/2; ++i)
    {
        res[i-1]=0;
        int left= std::atoi(argv[2*i-1]);
        int right= std::atoi(argv[2*i]);
        
        if (right < left) {
            //return -1;
            continue;
        }

        int left_idx=-1,right_idx=-1;

        for (int j=0; j<Size; ++j) {
            if (Data[j] == left) {
                left_idx=j;
                break;
            }
        }
        if (left_idx == -1) {
            continue;
        }
        for (int j=left_idx; j<Size; ++j) {
            if (Data[j] == right) {
                right_idx=j;
            }
            if (Data[j] > right) break;
        }
        if (right_idx == -1) {
            continue;
        }
        for (int j=left_idx; j<=right_idx; ++j) {
            res[i-1]+=is_prime(Data[j]);
        }
    }
    
    for (int i=0; i<(argc-1)/2; ++i) {
        std::cout<<res[i]<<"\n";
    }

    delete[] res;
        
    return 0;
}
