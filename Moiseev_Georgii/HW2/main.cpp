#include "calc.h"


int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        std::cout << "error" << std::endl;
        return 1;
    }


    try
    {
        Calculator calc(argv[1]);
        std::cout << calc.getAns() << std::endl;  
    }
    catch (std::exception& e) 
    {
        std::cout << "error" << std::endl;
        return 1;
    }

    return 0;
}