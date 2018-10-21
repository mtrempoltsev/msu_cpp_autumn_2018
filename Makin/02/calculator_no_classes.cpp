#include <iostream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


using std::cerr;
using std::endl;
using std::cout;
using std::string;

using std::invalid_argument;

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



Token getToken(const char*& tokens, int &value)
{
	while (const auto simbol = *tokens++)
	{
		switch (simbol)
		{
			case ' ': continue;
			case '+': return Token::Plus;
			case '-': return Token::Minus;
			case '*': return Token::Multiply;
			case '/': return Token::Divide;
		}

		//number
		if ((simbol >= '0') && (simbol <= '9'))
		{
			value = simbol - '0';
			while ((*tokens >= '0') && (*tokens <= '9'))
			{
				value = value * 10 + (*tokens++) - '0';
			}
			return Token::Number;
		}

		throw invalid_argument("");
		return Token::Invalid;
	}

	return Token::Eof;
}



int64_t get(const char*& tokens, Token& nextSimbol)
{
	int value = 0;
	nextSimbol = getToken(tokens, value);
	return value;
}



//sign
int64_t getSIGN(const char*& tokens, Token& nextSimbol)
{
	int number = get(tokens, nextSimbol);

	if( (nextSimbol != Token::Number) && (nextSimbol != Token::Minus))
	{
		throw invalid_argument("");
	}

	if(nextSimbol == Token::Number)
	{
 		get(tokens,nextSimbol);
		return number;
	}

	else if (nextSimbol == Token::Minus)
	{
		number = get(tokens,nextSimbol);
		if (nextSimbol == Token::Number)
		{
 			get(tokens,nextSimbol);
			return (-1)*number;
		}
		else
		{
			throw invalid_argument("");
		}
	}
	return 1;
}



int64_t getDIV_MUL(const char*& tokens, Token& nextSimbol)
{
	int result = getSIGN(tokens, nextSimbol);

	while (nextSimbol == Token::Multiply || nextSimbol == Token::Divide)
	{
		if (nextSimbol == Token::Divide)
		{
			int number = getSIGN(tokens, nextSimbol);
			if (number == 0)
			{
				throw invalid_argument("");
			}
			else
			{
				result /= number;
			}
		}
		else
		{
			int number = getSIGN(tokens, nextSimbol);
			result *= number;
		}
	}

	return result;
}



int64_t calculator(const char* tokens)
{
	Token nextSimbol;

	int result = getDIV_MUL(tokens, nextSimbol);

	while (nextSimbol == Token::Plus || nextSimbol == Token::Minus)
	{
		if (nextSimbol == Token::Plus)
		{
			result += getDIV_MUL(tokens, nextSimbol);
		}

		else
		{
			result -= getDIV_MUL(tokens, nextSimbol);
		}
	}
	return result;
}



int main(int argc, char **argv)
{
	int result;
	const char* err{"error"};
	if (argc != 2)
	{
		cerr << err << endl;
		return 1;
	}
	else
	{
		try
		{
	 		result = calculator(argv[1]);
		 	std::cout << result << std::endl;
		}
		catch(invalid_argument&)
		{
			cerr << err << endl;
			return 1;
		}
	}
	return 0;
}