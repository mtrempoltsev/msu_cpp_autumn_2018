#pragma once
#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';
public:
	explicit Serializer(ostream& out)
		: out_(out)
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args)
	{
		return process(args...);
	}

private:
	ostream& out_;
	template <class T, class... Args>
	Error process(T val, Args... args)
	{
		if (process(val) == Error::NoError)
			return process(args...);
		else
			return Error::CorruptedArchive;
	};
	Error process()
	{
		return Error::NoError;
	};
	Error process(bool val)
	{
		if (val)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;
		return Error::NoError;
	};
	Error process(uint64_t val)
	{
		out_ << val << Separator;
		return Error::NoError;
	};
};

class Deserializer
{
public:
	explicit Deserializer(istream& in)
		: in_(in)
	{
	}

	template <class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&... args)
	{
		return process(args...);
	}

private:
	istream& in_;
	template <class T, class... Args>
	Error process(T& val, Args&... args)
	{
		if (process(val) == Error::NoError)
			return process(args...);
		else
			return Error::CorruptedArchive;
	};
	Error process()
	{
		return Error::NoError;
	};
	Error process(bool& val)
	{
		string str;
		in_ >> str;
		if (str == "true")
			val = true;
		else if (str == "false")
			val = false;
		else
			return Error::CorruptedArchive;
		return Error::NoError;
	}
	Error process(uint64_t& val)
	{
		string str;
		in_ >> str;
		try
		{
			if (str[0] == '-')
				throw invalid_argument("");
			val = stoull(str.c_str());
		}
		catch (exception& e)
		{
			return Error::CorruptedArchive;
		}
		return Error::NoError;
	}
};
