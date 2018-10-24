#include <iostream>
#include <cstring>
using namespace std;
class Calculator {
    const char* s;
public:
    Calculator(const char* x):s(x){}
    int64_t Calculate() {
        int64_t number1, number2;
        char operand;
        number1 = multiply_and_division();
        while (true) {
            if ((*s) == '\0')
            {
                break;
            }
            operand = math_sign();
            number2 = multiply_and_division();
            if (operand == '+')
                number1 += number2;
            else
                number1 -= number2;
        }
        return number1;
    }
private:
    char math_sign(){
        if ((*s != '*') && (*s != '/') && (*s != '+') && (*s != '-'))
        {
            throw invalid_argument("Error in math sign");
        }
        return *s++;
    }
    int64_t number() {
        int signum = 1;
        int64_t help = 0;
        while (*s == '-')
        {
            signum *= -1;
            s++;
        }
        if ((*s > '9') || (*s < '0') || (*s == '\0'))
        {
            throw invalid_argument("Error in number");
        }
        while (*s != '\0' && isdigit(*s))
        {
            help = help * 10 + *s++ - '0';
        }
        return help * signum;
    }
    int64_t multiply_and_division(){
        int64_t number1 = number(), number2;
        char operand;
        while(true) {
            if ((*s) == '\0')
            {
                break;
            }
            operand = math_sign();
            if ((operand != '*') && (operand != '/')) {
                s--;
                break;
            }
            number2 = number();
            if (operand == '*')
                number1 *= number2;
            else {
                if (number2 == 0)
                    throw invalid_argument("Division Zero");
                number1 /= number2;
            }
        }
        return number1;
    }
};
void remove_space(char * x, char* c)
{
    int i,j=0,line_lenght = strlen(x)+1;
    for(i = 0; i < line_lenght; i++)
    {
        if (x[i] != ' ')
        {

            c[j] = x[i];
            j++;
        }
    }
}

int main(int argc, char* argv[])

{
    if (argc != 2)
    {
         cout << "error" <<endl;
         return 1;
    }
    char *c;
    c = new char[strlen(argv[1])+1];
    try
    {
        remove_space(argv[1],c);
        Calculator a = Calculator(c);
        cout << a.Calculate() << endl;
        delete []c;
    }
    catch (exception&)
    {
        cout << "error" << endl;
        delete []c;
        return 1;

    }
    return 0;
}
