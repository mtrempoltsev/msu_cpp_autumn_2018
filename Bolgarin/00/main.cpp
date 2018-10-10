#include "header.h"

int main(int argc, char* argv[]) {

	if (argc < 2)
		std::cout << "Argument 'size' not found" << std::endl;
	else {	
		size_t size = std::stoi(argv[1]);

		std::cout << "Sum by rows: ";
		sum_by_rows(size);

		std::cout << "Sum by cols: ";
		sum_by_cols(size);
	}
	return 0;
}