#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cctype>
#include <stdexcept>

using namespace std;

template<class T>
void parse(vector<string>& string_args, T&& arg) {
	ostringstream out;
	out << arg;
	string_args.push_back(out.str());
}

template<class T, class... Args>
void parse(vector<string>& string_args, T&& arg, Args&&... args) {
	parse(string_args, arg);
	parse(string_args, forward<Args>(args)...);
}

string format(const string& str) {
	return str;
}

template <class... ArgsT>
string format(const string& str, ArgsT... args) {
	vector<string> string_args;
	parse(string_args, forward<ArgsT>(args)...);
	ostringstream out;
	size_t i = 0;
	while (i < str.length()) {
		if (str[i] == '}')
			throw runtime_error("error: unexpected }");
		if (str[i] == '{') {
			unsigned number_arg = 0;
			i++;
			if (!isdigit(str[i]))
				throw runtime_error("error: empty {}");
			while ((str[i] != '}') && (i < str.length())) {
				if (!isdigit(str[i]))
					throw runtime_error("error: format in {}");
				number_arg = number_arg * 10 + str[i] - '0';
				i++;
			}
			if (str[i] != '}')
				throw runtime_error("error: expected }");
			if (number_arg >= string_args.size())
				throw runtime_error("error: not such argument");
			out << string_args[number_arg];
		}
		else
			out << str[i];
		i++;
	}
	return out.str();
}
