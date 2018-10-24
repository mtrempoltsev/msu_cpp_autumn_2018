#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <algorithm>


class Calculator
{
private:
	std::stringstream in;
	char now = 0;
	int64_t v;
	int64_t answer;
	
	char check()
	{
		char c;
		if (!in.get(c)) 
			if (in.eof()) 
				return now = 0;
			else 
				throw std::invalid_argument("Exception");
        switch (c)
        {
			case 0:	return now = 0;
			case '+': return now = '+';
			case '-': return now = '-';
			case '*': return now = '*';
			case '/': return now = '/';
			default:
				in.putback(c);
				in >> v;
				return now = 1;
		}
	}

	int64_t near(bool flag)
	{
		if (flag) check();
		switch (now)
		{
			case '-': 
				{
					return -near(true);
				}
			case 1:
				{
					int64_t buf = v;
					check();
					return buf;
				}
			default:
				throw std::invalid_argument("Unexpected symbol");
				return 0;
		}
	}

	int64_t next(bool flag)
	{
		int64_t l = near(flag);
		while (true)
		{
			switch (now)
			{
				case '*': { l *= near (true); break; }
				case '/':
				{
					int64_t d = near(true);
					if (d == 0) throw std::invalid_argument ("Division by zero");
					l /= d; break;
				}
				default: return l;
			}
		}
	}

	int64_t start (bool flag)
	{
		int64_t l = next(flag);
		while (true)
		{
			switch (now)
			{
				case '+': l += next(true); break; 
				case '-': l -= next(true); break; 
				default: return l;
			}
		}
	}

public:

	bool setExpression(const char* s)
	{
		std::string str(s);
		str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );
		in = std::stringstream(str);
		try
		{
			check();
			if (now == 0) answer = 0;
			else answer = start(false);
			return true;
		}
		catch (std::exception& e)
		{
			return false;
		}
	}
	
	bool setExpression(std::string str)
	{
		str.erase( std::remove( str.begin(), str.end(), ' ' ), str.end() );
		in = std::stringstream(str);
		try
		{
			check();
			if (now == 0) answer = 0;
			else answer = start(false);
			return true;
		}
		catch (std::exception& e)
		{
			std::cout << "error" << std::endl;
			return false;
		}
	}

	int64_t getAnswer()
	{
		return answer;
	}
};


int main (int argc, char* argv[])
{
	if (argc == 1) { std::cout << "error" << std::endl; return 1; }
	Calculator calc;
	if (argc == 2)
	{
		if(calc.setExpression(argv[1]))
			std::cout << calc.getAnswer() << std::endl;
		else
		{
			std::cout << "error" << std::endl;
			return 1;
		}
		return 0;
	}
    std::cout << "error" << std::endl; return 1;
    return 1;
}
