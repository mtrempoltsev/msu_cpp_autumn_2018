#pragma once

#include <iostream>
#include <string>

using namespace std;

enum class Error {
	NoError,
	CorruptedArchive
};

class Serializer {
private:
	ostream& out_;
	static constexpr char sep = ' ';
public:
	explicit Serializer(ostream& out)
		: out_(out)
	{
	}

	template <class T>
	Error save(T& value) {
		return value.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args)
	{
		return process(args...);
	}

private:	
   	Error process(bool val) {
		if (val) {
			out_ << "true" << sep;
			return Error::NoError;
		}
		out_ << "false" << sep;
		return Error::NoError;
	}

	Error process(uint64_t val) {
		out_ << val << sep;
		return Error::NoError;
	}

	template <class T>
	Error process(T value) {
		return Error::CorruptedArchive;
	}
	
	template <class T, class... ArgsT>
	Error process(T value, ArgsT... args) {
		if (process(value) == Error::NoError) {
			return process(args...);
		}
		return Error::CorruptedArchive;
	}
};

class Deserializer {
private:
	istream& in_;
	static constexpr char Separator = ' ';

public:
	Deserializer(istream& in)
		: in_(in) 
	{
	}

	template<class T>
	Error load(T& object) {
		return object.serialize(*this);
	}

	template<class... ArgsT>
	Error operator()(ArgsT&... args) {
		return process(args...);
	}

private:
	Error process(bool& value) {
		string s;
		in_ >> s;
		if (s == "true") {
			value = true;
			return Error::NoError;
		} 
		if (s == "false") {
			value = false;
			return Error::NoError;
		}
		return Error::CorruptedArchive;
	}

	Error process(uint64_t& value) {
		string s;
		in_ >> s;
		if ('0' <= s[0] && '9' >= s[0]) {
			value = stoull(s);
			return Error::NoError;
		}
		return Error::CorruptedArchive;
	}

	template <class T>
	Error process(T& value) {
		return Error::CorruptedArchive;
	}

	template <class T, class... ArgsT>
	Error process(T& value, ArgsT&... args) {
		if (process(value) == Error::NoError) {
			return process(args...);
		}
		return Error::CorruptedArchive;
	}
};
