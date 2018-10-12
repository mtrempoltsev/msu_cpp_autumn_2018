#include <iostream>
#include <memory>
#include "numbers.dat"


/*void fill(auto& array, size_t size, auto fill) {
	for (size_t i = 0; i < size; i++)
		array[i] = fill;
}*/

/*void fillTrue(std::unique_ptr<bool[]>& array, size_t size) {
	for (size_t i = 0; i < size; i++)
		array[i] = true;
}*/

auto getPrimeIndicator(size_t maxNumber) {

	auto primeIndicator = std::make_unique<bool[]>(maxNumber + 1);
	for (size_t i = 0; i < maxNumber + 1; i++)
		primeIndicator[i] = true;

	primeIndicator[0] = false;
	primeIndicator[1] = false;

	for (size_t p = 2; p < maxNumber + 1; ) {

		for (size_t j = 2*p; j < maxNumber + 1; j+=p) 
			primeIndicator[j] = false;
		
		while (p < maxNumber + 1) 
			if (primeIndicator[++p] == true)
				break;
	}
	return primeIndicator;
}


int main(int argc, char* argv[])
{
	const int nOfPairs = (argc - 1) / 2;

	if (nOfPairs == 0 || argc % 2 == 0) {
		std::cout << -1;
		return -1;
	}


	auto primeIndicator = getPrimeIndicator(Size);

    for (int k = 1; k < nOfPairs*2; k+=2)
    {
        int start = std::atoi(argv[k]);
        int finish = std::atoi(argv[k+1]);

        if (start > finish) {
        	std::cout << 0;
        	return 0;
        }



        size_t indexMin = -1;
        size_t indexMax = Size;

        for (size_t i = 0; i < Size; i++) {
        	if (Data[i] == start) {
        		indexMin = i;
        		break;
        	}
        }

        if (indexMin == -1) {
        	std::cout << 0;
        	return 0;
        }

        for (size_t i = indexMin; i < Size; i++) {
        	if (Data[i] == finish) {
        		if (i+1 >= Size) {
        			indexMax = i;
        			break;
        		} else if (Data[i+1] == finish) {
        			continue;
        		}
        		indexMax = i;
        		break;
        	}
        }

        if (indexMax == Size) {
        	std::cout << 0;
        	return 0;
        }



        int primeCounter = 0;

        for (size_t i = indexMin; i <= indexMax; i++) {
        	if (primeIndicator[Data[i]]) 
        		primeCounter++;
        }

        std::cout << primeCounter << std::endl;
    }

    return 0;
}