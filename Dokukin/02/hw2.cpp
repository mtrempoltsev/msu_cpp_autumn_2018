#include <iostream>
#include <string>
#include <stack>

using namespace std;
using i64 = int64_t;
using ui64 = unsigned int64_t;

class Calculator
{
	typedef enum{usual, num, sign, error} status;
	
	bool Parse(const string& input, ui64 cur_pos, stack<i64>& numbers, \
	           stack<char>& signs, i64 cur_num, char cur_sign, status stat)
	{
		if (cur_pos < input.length())
		{
			char c = input[cur_pos];
			switch(stat)
			{
				case usual:
					if (whoAmI(c, numbers.empty(), signs, cur_sign, stat))
					{
						if (stat == num) cur_num = c - 48;
						cur_pos += 1;
						return Parse(input, cur_pos, numbers, signs, \
									 cur_num, cur_sign, stat);
					}
					else return false;	
					break;
					
				case num:
					if (whoAmI(c, numbers.empty(), signs, cur_sign, stat))
					{	
						if (stat == num)
						{
							cur_num *= 10;
							cur_num += c - 48;
						}
						else
						{
							numbers.push(cur_num);
							cur_num = 0;
						}
						
						cur_pos += 1;
						return Parse(input, cur_pos, numbers, signs, \
									 cur_num, cur_sign, stat);
					}
					else return false;	
					break;
					
				case sign:
					if (whoAmI(c, numbers.empty(), signs, cur_sign, stat))
					{
						if (stat == num)
						{
							cur_num = c - 48;
							if (numbers.empty())
							{
								if (cur_sign == '-') cur_num *= -1;
							}
							else signs.push(cur_sign);
							cur_sign = '!';
						}
						
						if (stat == usual) stat = sign;
						
						cur_pos += 1;
						return Parse(input, cur_pos, numbers, signs, \
									 cur_num, cur_sign, stat);
					}
					else return false;	
					break;

				case error:
					return false;
					break;	
			}
		}
		else 
		{
			switch(stat)
			{
				case usual:
					return true;
					break;

				case num:
					numbers.push(cur_num);
					return true;
					break;
					
				case sign:	
				case error:
					return false;
					break;
			}
		}
		return true;
	}
	
	bool whoAmI(const char& c, const bool numbers_empty, \
				stack<char>& signs, char& cur_sign, status& stat)
	{
		if ((c >= 48) && (c <= 57))
		{
			stat = num;
			return true;
		}
		else if (c == ' ')
		{
			stat = usual;
			return true;
		}
		else if ((c == '-') || (c == '+'))
		{
			if (stat == sign)
			{
				if ((cur_sign == '-') || (cur_sign == '+'))
				{		 
					if (cur_sign != c) cur_sign = '-';
					else cur_sign = '+';
					return true;
				}
				else
				{
					stat = error;
					return false;
				}
			}
			else
			{
				cur_sign = c;
				stat = sign;
				return true;
			}
		}
		else if ((c == '/') || (c == '*'))
		{
			if ((stat != sign) && ((numbers_empty == false) || (stat == num)))
		    {
				cur_sign = c;
				stat = sign;
				return true;
			}
			else
			{
				stat = error;
				return false;
			}
		}
		else stat = error;
		return false;
	}
	
	bool CalculateResult(stack<i64>& numbers, stack<char>& signs, i64& result)
	{
		char cur_sign;
		i64 num1, num2;
		
		result = 0;		
		
		while ((numbers.size() != 1) && (signs.size() != 0))
		{
			num1 = numbers.top();
			numbers.pop();
			
			cur_sign = signs.top();
			signs.pop();
			
			if (cur_sign == '+')
			{
				result += num1;
				continue;
			}
			
			if (cur_sign == '-')
			{
				result -= num1;
				continue;
			}			
			
			if ((cur_sign == '*') || (cur_sign == '/'))
			{
				if (numbers.size() != 0)
				{
					num2 = numbers.top();
					numbers.pop();
					if (cur_sign == '*') num2 *= num1;
					else if (num1 != 0) num2 /= num1;
					else return false;
					numbers.push(num2);
				}	
				else return false;	
				continue;
			}
		}
		
		if (signs.size() == 0)
		{
			result += numbers.top();
			return true;
		}
		else return false;
		
		return true;
	}
	
public:
	
	bool Calculate(const string& input, i64& result)
	{
		status stat = usual;
		stack<i64> numbers;
		stack<char> signs;
		
		if (!Parse(input, 0, numbers, signs, 0, '!', stat)) return false;
		return CalculateResult(numbers, signs, result);
	}
};

i64 raiseError()
{
	cout << "error" << endl;
	return 1;
}

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		Calculator calc;
		i64 result = 0;
		if (calc.Calculate(argv[1], result)) cout << result << endl;
		else return raiseError();
	}
	else return raiseError();
	return 0;
}
