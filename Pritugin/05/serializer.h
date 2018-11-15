// serializer.h
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
	Error process()
	{
		return Error::NoError;
	}
	
	Error process(bool val)
	{
		if(val)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;
		return Error::NoError;
	}
	Error process(uint64_t val)
	{
		out_ << val << Separator;
		return Error::NoError;
	}
	template <class T, class... Args>
	Error process(T val, Args&&... args)
	{
		if(process(val) == Error::NoError)
			return process(std::forward<Args>(args)...);
		else
			return Error::CorruptedArchive;
	}
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
	Error operator()(ArgsT&&... args)
	{
		return process(args...);
	}
	
private:
	std::istream& in_;
	Error process(bool& value)
	{
		std::string text;
		in_ >> text;
		if (text == "true")
			value = true;
		else if (text == "false")
			value = false;
		else
			return Error::CorruptedArchive;

		return Error::NoError;
	}
	
	Error process()
	{
		return Error::NoError;
	}
	
	Error process(uint64_t& value)
	{
		std::string text;
		in_ >> text;

		try
		{
			if(text[0] == '-')
				throw std::invalid_argument("Minus");
			value = std::stoull(text.c_str());
		}
		catch(std::invalid_argument)
		{
			return Error::CorruptedArchive;
		}

		return Error::NoError;
	}

	template <class T, class... Args>
	Error process(T& val, Args&&... args)
	{
		if(process(val) == Error::NoError)
			return process(std::forward<Args>(args)...);
		else
			return Error::CorruptedArchive;
	}
};


