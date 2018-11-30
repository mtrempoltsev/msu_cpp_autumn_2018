#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>


using std::to_string;

template<typename T>
std::string to_string(const T &t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

// ----------------

std::string format(const std::string& s)
{
	return s;
}

template<class... Args>
std::string format(const std::string& s, Args&&... args)
{
	std::vector<std::string> strs {to_string(std::forward<Args>(args))...};
	
	static const char FORMAT_SYMBOL = '{';
	std::string res;
	std::string buf;
	bool arg = false;
	for (size_t i = 0; i <= s.size(); ++i)
	{
		bool last = (i == s.size());
		if(arg && last)
			throw std::runtime_error("Error");
		if (arg)
		{
			if (isdigit(s[i]))
			{
				buf += s[i];
			}
			else
			{
				if(s[i] == '}')
				{	
					size_t num = 0;
					if (!buf.empty())
						num = atoi(buf.c_str());
					else
						throw std::runtime_error("Error");
						
					if (num >= 0 && num < strs.size())
						res += strs[num];
					else
						throw std::runtime_error("Error");
					buf.clear();
					arg = false;
				}
				else
				{
					throw std::runtime_error("Error");
				}
			}
		}
		else
		{
			if (s[i] == FORMAT_SYMBOL)
			{
				arg = true;
			}
			else
			{
				if(s[i] == '}')
					throw std::runtime_error("Error");
				if (!last)
					res += s[i];
			}
		}
	}
	return res;
}


