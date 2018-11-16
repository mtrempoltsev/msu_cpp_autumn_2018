#include <string.h>
#include <stdexcept>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <sstream>

using namespace std;

enum State
{
	normal,
	bracket
}; 

int64_t parse(const string& str, vector<string>& substrings)
{
	int64_t nargs_expected = -1;
	State state = normal;
	string temp_str = "";
	string temp_substr = "";
	
	for (const char& c : str)
	{
		if (c == '{')
		{
			if (state == normal)
			{
				state = bracket;
				if (temp_substr != "")
				{
					substrings.push_back(temp_substr);
				}
				temp_substr = "{";
			}
			else
			{
				throw runtime_error("Error in parse: two following {");
			}
		}
		else
		{
			if (c == '}')
			{
				if (state == bracket)
				{
					int64_t narg = stoull(temp_str);
					temp_str = "";					
					state = normal;
					
					if (narg > nargs_expected)
					{
						nargs_expected = narg;
					}
					
					temp_substr += c;
					substrings.push_back(temp_substr);
					temp_substr = "";
				}
				else
				{
					throw runtime_error("Error in parse: } without {");
				}
			}
			else
			{
				temp_substr += c;
				if (state == bracket)
				{
					if ((c < 48) || (c > 57))
					{
						throw runtime_error("Error in parse: NAN in {}");
					}
					temp_str += c;
				}
			}
		}
	}
	
	if (state == bracket)
	{
		throw runtime_error("Error in parse: } not found");
	}
	
	if (temp_substr != "")
	{
		substrings.push_back(temp_substr);
	}
		
	return 	nargs_expected;	
}

void my_format(vector<string>& strings, const int64_t& nargs_expected)
{
	throw runtime_error("Error in my_format: not enough args");
}

template <class T, class... Args>
void my_format(vector<string>& strings, const int64_t& nargs_expected,
				 T&& value, Args&&... args)
{
	int64_t narg = nargs_expected + 1;
	
	stringstream res;
	res << value;
	strings.push_back(res.str());
	
	if (strings.size() < narg)
	{
		my_format(strings, nargs_expected, std::forward<Args>(args)...);
	}
}

string my_parse(vector<string>& substrings, vector<string>& strings)
{
	stringstream res;
	
	for (int i = 0; i < substrings.size(); ++i)
	{
		if (substrings[i][0] != '{')
		{
			res << substrings[i];
		}
		else
		{
			string str_num = (substrings[i]).substr(1,
										   substrings[i].length() - 1);
			int64_t num = stoull(str_num);
			res << strings[num];
		}
	}
	
	return res.str();	
}

template <class... Args>
string format(const string& str, Args&&... args)
{
	vector<string> substrings;
	int64_t nargs_expected = parse(str, substrings);
	if (nargs_expected == -1)
	{
		return str;
	}
	else
	{
		vector<string> strings;
		my_format(strings, nargs_expected, std::forward<Args>(args)...);
		string res = my_parse(substrings, strings);
		return res;
	}
}
