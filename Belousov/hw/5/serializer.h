#pragma once
#include <cstdint>
#include <string>
#include <type_traits>

enum class Error {
	NoError,
	CorruptedArchive
};

class Serializer {
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream & out) : out_(out) {}

	template <typename T>
	Error save(T & object) {
		Error err = object.serialize(*this);
		out_ << Separator;
		return err;
	}

	template <typename ... Args>
	Error operator()(Args ... args) {
		return process(args ...);
	}

private:
	Error process(bool value) {
		if (value)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;

		return Error::NoError;
	}

	Error process(uint64_t value) {
		out_ << value << Separator;
		return Error::NoError;
	}

	template <typename T>
	Error process(T value) {
		return Error::CorruptedArchive;
	}

	template <typename T, typename ... Args>
	Error process(T value, Args ... args) {
		Error err1 = process(value);
		if (err1 != Error::NoError)
			return err1;

		Error err2 = process(args ...);
		if (err2 != Error::NoError)
			return err2;

		return Error::NoError;
	}

	std::ostream & out_;
};

class Deserializer {
public:
	explicit Deserializer(std::istream & in) : in_(in) {}

	template <typename ... Args>
	Error operator()(Args & ... args) {
		return process(args ...);
	}

	template <typename T>
	Error load(T & object) {
		return object.serialize(*this);
	}

private:
	Error process(bool & value) {
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

	Error process(uint64_t & value) {
		std::string text;
		in_ >> text;
 
		if (text[0] == '-')
			return Error::CorruptedArchive;

		try {
			value = std::stoull(text);
		}
		catch (std::invalid_argument &) {
			return Error::CorruptedArchive;
		}

		return Error::NoError;
	}

	template <typename T>
	Error process(T & value) {
		return Error::CorruptedArchive;
	}

	template <typename T, typename ... Args>
	Error process(T & value, Args & ... args) {
		Error err1 = process(value);
		if (err1 != Error::NoError)
			return err1;

		Error err2 = process(args ...);
		if (err2 != Error::NoError)
			return err2;

		return Error::NoError;
	}

	std::istream & in_;
};