#pragma once

#include <iostream>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	std::ostream& out_;
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

	template <class... Args>
	Error operator()(Args... args)
	{
		return process(args...);
	}

private:
	Error process(bool val)
	{
		if (val) {
			out_ << "true" << Separator;
		} else {
			out_ << "false" << Separator;
		}

		return Error::NoError;
	}

	Error process(uint64_t val)
	{
		out_ << val << Separator;

		return Error::NoError;
	}

	template <class T>
	Error process(T val)
	{
		return Error::CorruptedArchive;
	}

	template <class T, class... Args>
	Error process(T current, Args... args)
	{
	if (process(current) == Error::NoError) {
		return process(args...);
	}
		return Error::CorruptedArchive;
	}
};

class Deserializer
{
	std::istream& in_;
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


	template <class... Args>
	Error operator()(Args&... args)
	{
		return process(args...);
	}

private:
	Error process(bool& value)
	{
		std::string text;
		in_ >> text;

		if (text == "true") {
			value = true;
		} else if (text == "false") {
			value = false;
		} else {
			return Error::CorruptedArchive;
		}

		return Error::NoError;
	}

	Error process(uint64_t& value)
	{
		std::string text;
		in_ >> text;

		if (text[0] == '-') {
			return Error::CorruptedArchive;
		}

		try {
			value = stoull(text);
		}
		catch(std::invalid_argument &) {
			return Error::CorruptedArchive;
		}

		return Error::NoError;
	}

	template <class T>
	Error process(T& object)
	{
		return Error::CorruptedArchive;
	}

	template <class T, class... Args>
	Error process(T& current, Args&... args)
	{
	if (process(current) == Error::NoError) {
		return process(args...);
	}
		return Error::CorruptedArchive;
	}
};