#include "numbers.dat"
#include <vector>
#include <algorithm>
#include <iostream>

// O(NloglogN) + C*O(logN) + C*O(N) = O(NloglogN)
int main(int argc, char* argv[])
{
	// O(NloglogN)
	if((argc == 1) or !(argc % 2)) return -1;
	
	const int n = 1000000;
	std::vector<char> prime (n+1, true);
	prime[0] = prime[1] = false;
	for (int i = 2; i*i <= n; ++i)
		if (prime[i] && i*i <= n)
			for (int j = i*i; j <= n; j+=i) 
				prime[j] = false;

				
	for (int i = 1; i < argc; ++i)
	{
		auto v1 = std::atoi(argv[i]);
		auto v2 = std::atoi(argv[++i]);
		if(v1 > v2) { std::cout << 0 << std::endl; continue; }
		// O(logN)
		auto left = std::lower_bound(Data, Data + Size, v1);
		auto right = std::upper_bound(Data, Data + Size, v2);
		right--;
		
		if(*right != v2) { std::cout << 0 << std::endl; continue; }
		if(*left != v1) { std::cout << 0 << std::endl; continue; }

		int count = 0;
		// O(N)
		for(; left < right; ++left) count += prime[*left];	
		// answer
		std::cout<< count << std::endl;
	}
	return 0;
}
