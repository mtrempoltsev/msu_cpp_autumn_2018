#pragma once
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <regex>
#include <string>

bool is_positive_number(const std::string& str) {
	static std::regex zero_regex("^\\s*0\\s*$");
	static std::regex number_regex("^\\s*[1-9][0-9]*\\s*$");
	return std::regex_match(str, zero_regex) || std::regex_match(str, number_regex);
}

template<typename T>
std::string to_string(T&& arg) {
	std::stringstream ss;
	std::string s;
	ss << arg;
	ss >> s;
	return s;
}

class Pattern {
private:
	std::vector<std::shared_ptr<std::string>> body;
	std::map<size_t, std::shared_ptr<std::string>> args;
private:
	bool is_valid_parentheses(const std::string& pattern) const {
		std::stack<bool> s;
		for (const char& c : pattern) {
			if (c == '{') {
				s.push(true);
			} else if (c == '}') {
				if (s.empty()) {
					return false;
				}
				s.pop();
			}
		}
		return s.empty();
	}
public:
	explicit Pattern(const std::string& pattern) {
		std::stringstream ss(pattern);
		std::string line;
		if (!is_valid_parentheses(pattern)) {
			throw std::runtime_error("invalid parentheses");
		}
		if (pattern.find("{") == std::string::npos && pattern.find("}") == std::string::npos) {
			body.push_back(std::make_shared<std::string>(pattern));
		} else {
			while (std::getline(ss, line, '{')) {
				body.push_back(std::make_shared<std::string>(line));
				std::getline(ss, line, '}');
				if (!is_positive_number(line)) {
					throw std::runtime_error("invalid number");
				}
				int num = std::stoi(line);
				if (args.count(num)) {
					body.push_back(args[num]);
				} else {
					body.push_back(std::make_shared<std::string>(""));
					args[num] = body.back();
				}
			}
		}
	}

	std::string& operator[] (size_t num) {
		if (args.count(num) > 0) {
			return *args[num];
		} else {
			throw std::runtime_error("no argnums in pattern");
		}
	}

	size_t args_count() const {
		return args.size();
	}

	std::vector<size_t> get_args() const {
		std::vector<size_t> result;
		for (auto it = args.begin(); it != args.end(); ++it) {
			result.push_back(it->first);
		}
		return result;
	}

	std::string get_string() const {
		std::string res;
		for (const auto ptr : body) {
			res += *ptr;
		}
		return res;
	}
};

class PatternArgs {
private:
	std::vector<std::string> args;
public:
	size_t size() {
		return args.size();
	}
	size_t count(size_t num) {
		if (this->size() > num)
		{
			return 1;
		}
		return 0;
	}
	template<typename... ArgsT>
	explicit PatternArgs(ArgsT&&... pattern_args) {
		args = std::move(std::vector<std::string> {to_string(std::forward<ArgsT>(pattern_args))...});
	}

	const std::string& operator[] (size_t num) const {
		return args[num];
	}
};

class Formatter {
private:
	Pattern pattern;
	PatternArgs pattern_args;
	std::string body;
public:
	template <typename... ArgsT>
	explicit Formatter(const std::string& pattern_, ArgsT&&... pattern_args_)
		: pattern(pattern_), pattern_args(std::forward<ArgsT>(pattern_args_)...) {
		for (const size_t& i : pattern.get_args()) {
			if (pattern_args.count(i)) {
				pattern[i] = pattern_args[i];
			}
			else {
				throw std::runtime_error("invalid argnum");
			}
		}
		body = pattern.get_string();
	}

	std::string formatted_string() const {
		return body;
	}
};

template <typename... ArgsT>
std::string format(const std::string& pattern, ArgsT&&... pattern_args) {
	Formatter f(pattern, std::forward<ArgsT>(pattern_args)...);
	return f.formatted_string();
}