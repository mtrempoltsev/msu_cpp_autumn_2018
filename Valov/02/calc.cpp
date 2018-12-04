#include <cctype>
#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <sstream>
#include <string>
using namespace std;

enum Token_value : char 
{
	NUMBER, END,
	PLUS='+', MINUS='-', MUL='*', DIV='/',
	PRINT=';'
};

enum Number_value : char 
{
	NUM0='0', NUM1='1', NUM2='2',
	NUM3='3', NUM4='4', NUM5='5',
	NUM6='6', NUM7='7', NUM8='8',
	NUM9='9'
};

Token_value curr_tok = PRINT;
int64_t number_value;                 

int64_t expr(istream*, bool);

Token_value get_token(istream* input)
{
	char ch;
	do 
	{
		if (!input->get(ch))
			return curr_tok = END;
	}
	while (ch != '\n' && isspace(ch));
	switch (ch) 
	{
	case 0:
		return curr_tok = END;
    	case PRINT:
    	case '\n':
		return curr_tok = END;
	case MUL:
    	case DIV:
    	case PLUS:
    	case MINUS:
		return curr_tok = Token_value(ch);
    	case NUM0: case NUM1: case NUM2: case NUM3: case NUM4:
    	case NUM5: case NUM6: case NUM7: case NUM8: case NUM9:
		input->putback(ch);
		*input >> number_value;
		return curr_tok = NUMBER;
	default:
		throw runtime_error("");
	}
}

int64_t prim(istream* input, bool get) 
{
	if (get) 
	{
		get_token(input);
	}
	switch (curr_tok) 
	{
    	case NUMBER: 
		{
			int64_t v = number_value;
			get_token(input);
			return v;
		}
    	case MINUS:
		return -prim(input, true);
    	default:
		throw runtime_error("");
	}
}

int64_t term(istream* input, bool get) 
{
	int64_t left = prim(input, get);
	for ( ; ; ) 
	{
		switch (curr_tok) 
		{
		case MUL:
			left *= prim(input, true);
			break;
		case DIV:
			if (int64_t d = prim(input, true)) 
			{
				left /= d;
				break;
			}
			throw runtime_error("");
			break;
		default:
			return left;
		}
	}
}

int64_t expr(istream* input, bool get) 
{
	int64_t left = term(input, get);
	for ( ; ; ) 
	{
		switch (curr_tok) 
		{
		case PLUS:
			left += term(input, true);
			break;
		case MINUS:
		  	left -= term(input, true);
		  	break;
		default:
		  	return left;
		}
	}
}

int main(int argc, char* argv[]) 
{
	if (argc > 2 || argc == 1) 
	{
        	cout << "error" << endl;
        	return 1;
    	}
	istream* input = nullptr; 
	input = new istringstream(argv[1]);
	try 
	{
		while (*input) 
		{
			get_token(input);
			if (curr_tok == END)
				break;
			if (curr_tok == PRINT)
				throw runtime_error("");
			cout << expr(input, false) << endl;
		}
	} 
	catch (exception &error) 
	{
        	cout << "error"<< endl;
        	return 1;
    	}
}
