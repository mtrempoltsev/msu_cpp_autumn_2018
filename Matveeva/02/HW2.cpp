#include <iostream>
#include <cstring>
using namespace std;
class Calculator {
    const char* s;
    const int n;
public:
    Calculator(const char* st, const int m) : s(st), n(m){}
    ~Calculator()
    {
        delete [] (s - n);
    }
    int64_t calculation() 
    {
        int64_t res;
        res = monom();
        if (*s == '\0')
            return res;
        while (1) 
        {
            if (*s == '\0')
                return res;
            if (s[0] == '+')
            {
                s ++;
                res += monom();
            }
            else if (s[0] == '-')
            {
                s ++;
                res -= monom();
            }
        }
        return res;
    }
private:
    int64_t monom() 
    {
        int64_t monom = number();
        int64_t divisor;
        while(1) 
        {
            if (*s == '\0')
                return monom;
            if (s[0] == '*')
            {
                s ++;
                monom *= number();
            }
            else if (s[0] == '/')
            {
                s ++;
                divisor = number();
                if (divisor == 0)
                    throw invalid_argument("zero division");
                monom /= divisor;
            }
            else{
                break;
            }
        }
        return monom;
    }

    int64_t number() 
    {
        int sign = 1;
        int64_t num = 0;
        if (*s == '-') 
        {
            sign *= (-1);
            s ++;
        }
        if (!isdigit(*s) && *s != '\0')
        {
            throw invalid_argument("wrong symbol");
        }
        while (*s != '\0' && isdigit(*s))
        {
            num = num * 10 + *s - '0';
            s ++;
        }
        return (num * sign);
    }
};
void copy_str(char * str_without_space, char* s)
{
    int i,j = 0;
    for(i = 0; i < (strlen(s) + 1); i ++)
    {
        if (s[i] != ' ')
        {
            str_without_space[j] = s[i];
            j ++;
        }
    }
}

int main(int argc, char* argv[])
{
    char* st_without_space;
    if (argc != 2)
    {
        cout << "error" << endl;
        return 1;
    }
    try{
        int kol = 0;
        int m = strlen(argv[1]) + 1;
        for (int i = 0; i < m; i++)
        {
            if (argv[1][i] == ' ')
                kol ++;
        }
        st_without_space = new char[m - kol];
        copy_str(st_without_space, argv[1]);
        Calculator c = Calculator(st_without_space, m);
        cout << c.calculation() << endl;
    } catch (exception&){
        cout << "error" << endl;
        return 1;
    }
    delete[] st_without_space;
    return 0;
}
