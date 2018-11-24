#pragma once

#include <iostream>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out)
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
	std::ostream& out_;
	template <class T, class... Args>
	Error process(T val, Args&... args)
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
	explicit Deserializer(std::istream& in)
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
	std::istream& in_;
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
		std::string str;
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
		std::string str;
		in_ >> str;
		try
		{
			if (str[0] == '-')
				throw std::invalid_argument("");
			val = std::stoull(str.c_str());
		}
		catch (std::invalid_argument)
		{
			return Error::CorruptedArchive;
		}
		return Error::NoError;
	}
};
