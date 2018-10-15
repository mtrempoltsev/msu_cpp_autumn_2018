#include <iostream>
#include "numbers.dat"

bool checkPrime(int num)
{
	if(num <= 1)
		return false;

	for(int i = 2; i*i <= num; i++)
	{
		if(num % i == 0)
			return false;
	}

	return true;
}

int find(int ex)
{
	int index;
	for(index = 0; index < Size; index++)
		if(Data[index] == ex)
			return index;

	return -1;
}

int main(int argc, char** argv)
{
	if((argc - 1) % 2 != 0 || argc == 1)
		return -1;

	for(int i = 1; (i + 1) < argc; i += 2)
	{
		int left = atoi(argv[i]);
		int right = atoi(argv[i+1]);

		left = find(left);
		right = find(right);
		
		if(left == -1 || right == -1)
			std::cout << 0 << std::endl;
		else
		{
			int tmp = right;
			while(Data[tmp] == Data[right] && right < Size)
				right++;
			right--;

			int count = 0;
			
			for(int j = left; j <= right; j++)
				if(checkPrime(Data[j]))
					count++; 
			
			std::cout << count << std::endl;	
		}
	}
	return 0;
}