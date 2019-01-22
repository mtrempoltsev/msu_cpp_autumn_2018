#pragma once

#include <string>
#include <vector>
#include <sstream>


using namespace std;

template<class T>
std::string to_string(T arg) {
	std::stringstream sstr;
	sstr << arg;
	return sstr.str();
}

string make_new_string(vector<string> const&v, const char *str) {
	string res;
	bool in_brackets = false;
	size_t i = 0;
	size_t ind = 0;
	char el = str[0];

	while (el != '\0') {
		while (in_brackets && el != '\0' && isdigit(el)) {
			ind *= 10;
			ind += el - '0';
			i++;
			el = str[i];
		}

		if (in_brackets && el != '}') {
			throw std::runtime_error("");
		} else if (in_brackets) {
				if (ind >= v.size()) {
					throw std::runtime_error("");
				}
			res += v[ind];
			ind = 0;
			in_brackets = false;
		} else if (el == '{') {
			in_brackets = true;
		} else if (el == '}') {
			throw std::runtime_error("");
		} else {
			res += el;
		}

	 	i++;
	 	el = str[i];
	}

	if (in_brackets) {
		throw std::runtime_error("");
	}

	return res;
}

template <class... Args>
string format(const char *str, Args... args)
{
	vector<string> v{to_string(forward<Args>(args))...};

	return make_new_string(v, str);
}

string format(const char *str)
{
	return string(str);
}
