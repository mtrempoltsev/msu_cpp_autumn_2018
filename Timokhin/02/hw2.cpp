#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

class Calculator
{
public:
	Calculator(char* expr):
		m_ss(expr){}

	bool calculate(int64_t& result);
private:
	void readSpaces();
	bool parseNumber(int64_t& result);
	bool parseMulDivExpr(int64_t& result, int64_t arg, bool isMul);
	bool parseAddSubExpr(int64_t& result, int64_t arg, char sign);
	stringstream m_ss;
};

void Calculator::readSpaces()
{
	char c = ' ';
	while (isspace(c) && !m_ss.eof())
		m_ss >> c;
	if (!m_ss.eof() && !isspace(c))
		m_ss.unget();
}

bool Calculator::parseNumber(int64_t& result)
{
	readSpaces();
	char neg = ' ';
	char sign = 1;
	m_ss >> neg;
	while(neg == '-')
	{
		sign *= -1;
		readSpaces();
		m_ss >> neg;
	}
	m_ss.unget();
	m_ss >> result;
	result *= sign;
	return !m_ss.fail();
}

bool Calculator::parseMulDivExpr(int64_t& result, int64_t arg, bool isMul)
{
	bool ret = false;
	if (!parseNumber(result))
		return false;
	if (isMul)
	{
		result = result * arg;
	}
	else
	{
		if (result == 0)
			return false;
		result = arg / result;
	}
	readSpaces();
	char operation = ' ';
	m_ss >> operation;
	if (m_ss.eof())
		return true;
	switch (operation)
	{
	case '*':
		ret = parseMulDivExpr(result, result, true);
		break;
	case '/':
	{
		ret = (result != 0) && parseMulDivExpr(result, result, false);
		break;
	}
	case '+': case '-':
		m_ss.unget();
		ret = true;
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}

bool Calculator::parseAddSubExpr(int64_t& result, int64_t arg, char sign)
{
	if (!parseMulDivExpr(result, 1, true))
		return false;
	result = result * sign + arg;
	bool ret = false;
	readSpaces();
	char operation = ' ';
	m_ss >> operation;
	if (m_ss.eof())
		return true;
	switch (operation)
	{
	case '+':
		ret = parseAddSubExpr(result, result, 1);
		break;
	case '-':
		ret = parseAddSubExpr(result, result, -1);
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}

bool Calculator::calculate(int64_t& result)
{
	return parseAddSubExpr(result, 0, 1);
}

int main(int argc, char** argv)
{
	int64_t result = 0;
	if (argc != 2 || !Calculator(argv[1]).calculate(result))
	{
		cout << "error" << endl;
		return 1;
	}
		cout << result << endl;
	return 0;
}
