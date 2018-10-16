#include<iostream>
#include<cmath>
#include "numbers.dat"
using namespace std;

bool is_prime(int n, int p[])
{
    if(n!=1)
    {
        const int maxi_divisor=sqrt(n);
        for (int i=0; p[i]<=maxi_divisor; i++)
        {
            if(n%p[i]==0)
                return false;
        }
        return true;
    }
    else
        return false;
}

int bin_search(const int & a)
{
    int start=0;
    int stop = Size-1;
    if(Data[start]!=a)
    {
        int position;
        while(1)
        {
            if((stop-start)<=1)
                return -1;
            position=(start+stop)/2;
            if(Data[position]==a)
            {
                while(Data[position]==Data[position-1])
                {
                    position--;
                }
                return position;
            }
            if(Data[position]<a)
                start=position;
            else
                stop=position;
        }
    }
    else
        return 0;
}

int number_primes(const int & a, const int & b, int primes[])
{
    int i=bin_search(a);
    if(i==-1)
        return 0;
    int number=0;
    for(; i<Size && Data[i]<=b; i++)
    {
        if(is_prime(Data[i], primes))
            number++;
    }
    if(Data[i-1]<b)
        return 0;
    return number;
}


int main(int argc, char* argv[])
{
    if(argc<2)
    {
        cout<<0<<endl;
        return -1;
    }
    if(argc%2==0)
        return -1;
    int u, v;
    int *primes = new int [66] {2, 3, 5, 7,	11,	13,	17,	19,	23,	29,	31,	37,	41,	43,	47,	53,	59,	61,	67,	71,
    73,	79,	83,	89,	97,	101, 103, 107, 109,	113, 127, 131, 137,	139, 149, 151, 157,	163, 167, 173,
    179, 181, 191, 193,	197, 199, 211,	223, 227, 229, 233,	239, 241, 251, 257,	263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317};
    for (int i = 1; i < argc; i+=2)
    {
        u=atoi(argv[i]);
        v=atoi(argv[i+1]);
        if(u<=v)
            cout<<number_primes(u, v, primes)<<endl;
        else
            cout<<0<<endl;
    }
    delete[] primes;
    return 0;
}
