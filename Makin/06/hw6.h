#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

template<class T>
std::string to_string(T&& arg) 
{
	std::ostringstream output;
	output << arg;
	return output.str();
}

std::string format(const std::string& input_str)
{
	return input_str;
}

template <class... ArgsT>
std::string format(const std::string& input_str, ArgsT&&... args)
{
	std::vector<std::string> params{to_string(std::forward<ArgsT>(args))...};
	std::ostringstream output;
	size_t pos = 0;

	while (pos < input_str.length())
	{
		if (input_str[pos] == '}')
			throw std::runtime_error("error");

		if (input_str[pos] == '{') 
		{
			int num = 0;
			pos++;
			if (!isdigit(input_str[pos]))
				throw std::runtime_error("error");

			while ((input_str[pos] != '}') && (pos < input_str.length()))
			{
				if (!isdigit(input_str[pos]))
					throw std::runtime_error("error");
				num = input_str[pos] - 48;
				pos++;
			}
			if (input_str[pos] != '}')
				throw std::runtime_error("error");

			if (num >= params.size())
				throw std::runtime_error("error");
			output << params[num];
		}
		else
			output << input_str[pos];
		pos++;
	}
	return output.str();
}