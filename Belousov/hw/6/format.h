#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <sstream>	
#include <iostream>
#include <stdexcept>

class FormatData {
public:
	FormatData(std::string const & text) : _text(text), _result("") {
		make_result();
	}

	template <typename ... Args>
	FormatData(std::string const & text, Args && ... args)
		: _text(text), _result("") {
		std::vector<std::string> tmp_vec{to_string(std::forward<Args>(args)) ...};
		_args_vec = std::move(tmp_vec);
		make_result();
	}

	std::string const & get_result() const {
		return _result;
	}

private:
	template <typename T>
	std::string to_string(T && arg) {
		std::string tmp;
		_buf << arg << " ";
		_buf >> tmp;
		return tmp;
	}

	void make_result() {
		size_t pos = 0;

		for (size_t i = 0; i < _text.size(); i++) {
			while (_text[i] != '{' && i < _text.size()) {
				if (_text[i] == '}')
					throw std::runtime_error("bad data");
				i++;
			}

			_result += _text.substr(pos, i - pos);
			if (i >= _text.size())
				return;

			pos = i + 1;

			while (_text[i] != '}'&& i < _text.size())
				i++;

			if (i >= _text.size())
				throw std::runtime_error("bad data");

			int64_t idx;
			try {
				std::string tmp = _text.substr(pos, i - pos);
				size_t pos = 0;
				idx = std::stoll(tmp, &pos);
				if (pos != tmp.size())
					throw std::runtime_error("bad data");
			}
			catch(std::invalid_argument&) {
				throw std::runtime_error("bad data");
			}
			catch(std::out_of_range&) {
				throw std::runtime_error("bad data");
			}

			if (idx < 0 || idx >= _args_vec.size())
				throw std::runtime_error("bad data");

			_result += _args_vec[idx];
			pos = i + 1;
		}
	}

	std::vector<std::string> _args_vec;
	std::stringstream _buf;
	std::string _result;
	std::string const _text;
};

std::string format(std::string const & text) {
	FormatData data(text);
	return data.get_result();
}

template <typename ... Args>
std::string format(std::string const & text, Args && ... args) {
  FormatData data(text, std::forward<Args>(args) ...);
	return data.get_result();
}