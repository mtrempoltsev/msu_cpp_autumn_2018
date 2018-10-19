#include "numbers.dat"
#include <vector>
#include <algorithm>
#include <iostream>


// O(NloglogN)
std::vector<char> create_primes()
{
	const int n = 1000000;
	std::vector<char> prime (n+1, true);
	prime[0] = prime[1] = false;
	for (int i=2; i*i<=n; ++i)
		if (prime[i])
			if (i * i <= n)
				for (int j=i*i; j<=n; j+=i)
					prime[j] = false;
	return prime;
}


int solve(const char* str1, const char* str2)
{
	// O(NloglogN)
	static std::vector<char> prime = create_primes();
	auto v1 = std::atoi(str1);
	auto v2 = std::atoi(str2);
	//std:: cout<< v1 << " "<< v2 << std::endl;
	if(v1 > v2) return 0;
	// O(logN)
	auto left = std::lower_bound(Data, Data + Size, v1);
	auto right = std::upper_bound(Data, Data + Size, v2);
	right--;
		
	if((*right != v2) or (*left != v1)) return 0;

	int count = 0;
	// O(N)
	for(; left < right; ++left) count += prime[*left];	
	
	return count;
}


// O(NloglogN) + C*O(logN) + C*O(N) = O(NloglogN)
int main(int argc, char* argv[])
{
	if((argc == 1) or (argc % 2 == 0)) return -1;

	for (int i = 1 ; i < argc; i += 2) std::cout<< solve(argv[i], argv[i+1]) << std::endl;
	
	return 0;
}
