#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

template <class T>
string to_str(T&& arg)
{
	stringstream ss_arg;
	ss_arg << arg;
	return ss_arg.str();
}

template <class... T>
string format(const string& str, T&&... args)
{
	vector<string> str_args{ to_str(forward<T>(args))... };
	if (str_args.size() != 0)
	{
		ostringstream oss_result_str;
		unsigned int arg_pos = 0;
		for (int i = 0; i < str.length(); ++i)
		{
			if (str[i] == '{')
			{
				stringstream ss_arg_pos;
				while (i < str.length() && str[i + 1] != '}')
				{
					if (i + 2 >= str.length())
						throw runtime_error("error");
					if (str[i + 1] > '9')
						throw runtime_error("error");
					ss_arg_pos << str[i + 1];
					++i;
				};
				++i;
				arg_pos = stoi(ss_arg_pos.str());
				if (arg_pos >= str_args.size())
					throw runtime_error("error");
				oss_result_str << str_args[arg_pos];
			}
			else
			{
				if (str[i] == '}')
					throw runtime_error("error");
				oss_result_str << str[i];
			}
		}
		return oss_result_str.str();
	}
	else
		return str;
}