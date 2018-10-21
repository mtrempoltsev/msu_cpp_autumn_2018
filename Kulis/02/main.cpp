#include <iostream>
#include <exception>
#include "calculator.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return -1;
    }
    try {
        Calculator calc;
        std::cout << calc.calculate(argv[1]) << std::endl;
    } catch (std::exception) {
        std::cout << "error" << std::endl;
    }
	return 0;
}
