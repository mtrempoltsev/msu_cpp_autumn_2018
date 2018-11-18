#include <iostream>
#include <sstream>
#include <vector>

// standart types
std::string to_string(int x) 
{
	return std::to_string(x); 
}

std::string to_string(unsigned int x)
{
	return std::to_string(x);
}

std::string to_string(long x)
{
	return std::to_string(x);
}

std::string to_string(unsigned long x) 
{
	return std::to_string(x);
}

std::string to_string(long long x)
{
	return std::to_string(x);
}

std::string to_string(unsigned long long x)
{
	return std::to_string(x);
}

std::string to_string(float x)
{
	return std::to_string(x);
}

std::string to_string(double x)
{
	return std::to_string(x);
}

std::string to_string(long double x)
{
	return std::to_string(x);
}

std::string to_string(const char *x)
{
	return std::string(x);
}

std::string to_string(const std::string &x)
{
	return x;
}

template<typename T>
std::string to_string(const T &t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

// ----------------




std::string _format_(const std::string &s, const std::vector<std::string> &strs)
{
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
			if (s[i] >= '0' && s[i] <= '9')
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

template<class Arg, class... Args>
std::string _format_(const std::string& s, std::vector<std::string>& strs, Arg&& arg, Args&& ... args)
{
	strs.push_back(to_string(std::forward<Arg>(arg)));
	return _format_(s, strs, std::forward<Args>(args)...);
}


std::string format(const std::string& s)
{
	return s;
}

template<class Arg, class... Args>
std::string format(const std::string& s, Arg&& arg, Args&&... args)
{
	std::vector<std::string> strs;
	return _format_(s, strs, std::forward<Arg>(arg), std::forward<Args>(args)...);
}


