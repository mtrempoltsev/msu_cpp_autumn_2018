#include "calc.h"


int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        std::cout << "error" << std::endl;
        return 1;
    }


    calculator Calc(argv[1]);

    if (Calc.printAns() == false)
        return 1;
    else
        return 0;
}