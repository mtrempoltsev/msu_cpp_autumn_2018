#include <iostream>
#include <stdexcept>


using std::endl;
using std::cout;
using std::invalid_argument;

using Int = int64_t;


enum class Token
{
	Plus,
	Minus,
	Multiply,
	Divide,
	Number,
	Invalid,
	Eof
};


class Tokenizer
{
public:

	void set(const char* tokens)
	{
		this->tokens = tokens;
		value = 0;
		Current = Token::Invalid;
	}

	Int getToken()
	{
		while (const auto simbol = *tokens++)
		{
			switch (simbol)
			{
				case ' ': continue;
				case '+': Current = Token::Plus;
				return 0;
				case '-': Current = Token::Minus;
				return 0;
				case '*': Current = Token::Multiply;
				return 0;
				case '/': Current = Token::Divide;
				return 0;
			}

			//number
			if ((simbol >= '0') && (simbol <= '9'))
			{
				value = simbol - '0';
				while ((*tokens >= '0') && (*tokens <= '9'))
				{
					value = value * 10 + (*tokens++) - '0';
				}
				Current = Token::Number;
				return value;
			}

			throw invalid_argument("Error");
		}

		Current =  Token::Eof;
		return 0;
	}

	Token getCurrent() { return Current; }
	Int getValue() { return value; }
	
private:
	const char* tokens;
	Token Current;
	Int value;
};




class Calculator
{
public:

	Int get_calculate(const char* tokens)
	{
		tokenizer.set(tokens);
		return calculator();
	}

private:

	Tokenizer tokenizer;


	Int calculator()
	{
		Int result = getDIV_MUL();

		while (tokenizer.getCurrent() == Token::Plus || tokenizer.getCurrent()  == Token::Minus)
		{
			if (tokenizer.getCurrent()  == Token::Plus)
			{
				result += getDIV_MUL();
			}
			else
			{
				result -= getDIV_MUL();
			}
		}
		return result;
	}



	Int getDIV_MUL()
	{
		Int result = getSIGN();

		while (tokenizer.getCurrent()  == Token::Multiply || tokenizer.getCurrent()  == Token::Divide)
		{
			if (tokenizer.getCurrent()  == Token::Divide)
			{
				Int number = getSIGN();
				if (number == 0)
				{
					throw invalid_argument("Error");
				}
				else
				{
					result /= number;
				}
			}
			else
			{
				result *= getSIGN();
			}
		}

		return result;
	}


	//sign
	Int getSIGN()
	{
		Int number = tokenizer.getToken();

		if( (tokenizer.getCurrent() != Token::Number) && (tokenizer.getCurrent() != Token::Minus))
		{
			throw invalid_argument("Error");
		}

		if(tokenizer.getCurrent() == Token::Number)
		{
			tokenizer.getToken();
			return number;
		}

		else if (tokenizer.getCurrent() == Token::Minus)
		{
			number = tokenizer.getToken();
			if (tokenizer.getCurrent() == Token::Number)
			{
				tokenizer.getToken();
				return (-1)*number;
			}
			else
			{
				throw invalid_argument("Error");
			}
		}
		return 0;
	}
};


int main(int argc, char **argv)
{
	const char* err{"error"};
	if (argc != 2)
	{
		cout << err << endl;
		return 1;
	}
	else
	{
		try
		{
			Calculator calc;
		 	std::cout << calc.get_calculate(argv[1]) << std::endl;
		}
		catch(invalid_argument&)
		{
			cout << err << endl;
			return 1;
		}
	}
	return 0;
}