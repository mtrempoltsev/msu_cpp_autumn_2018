#pragma once

#include <iostream>
#include <string>

using namespace std;

enum class Error {
	NoError,
	CorruptedArchive
};

class Serializer {
public:
	explicit Serializer(ostream& out)
		: out_(out){}
	template <class T>
	Error save(T& object) {return object.serialize(*this);}
	template <class... ArgsT>
	Error operator()(ArgsT&&... args) {return process(forward<ArgsT>(args)...);}
private:
	static constexpr char Separator = ' ';
	ostream& out_;
	template <class T, class... ArgsT>
	Error process(T&& data, ArgsT&&... args) {
		if (process(data) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		out_ << Separator;
		return process(forward<ArgsT>(args)...);
	}
	Error process(bool data) {
		out_ << (data ? "true" : "false");
		return Error::NoError;
	}
	Error process(uint64_t data) {
		out_ << data;
		return Error::NoError;
	}
	template <class... ArgsT>
	Error process() {return Error::CorruptedArchive;}
};

class Deserializer {
public:
	explicit Deserializer(istream& in)
		: in_(in) {}
	template <class T>
	Error load(T& object) {return object.serialize(*this);}
	template <class... ArgsT>
	Error operator()(ArgsT&&... args) {return process(forward<ArgsT>(args)...);}
private:
	istream& in_;
	template <class T, class... ArgsT>
	Error process(T&& data, ArgsT&&... args) {
		return (process(data) == Error::CorruptedArchive)?
		(Error::CorruptedArchive):(process(forward<ArgsT>(args)...));
	}
	Error process(bool& data) {
		string text;
		in_ >> text;
		if (text == "true")
			data = true;
		else
			if (text == "false")
				data = false;
			else
				return Error::CorruptedArchive;
		return Error::NoError;
	}
	Error process(uint64_t& data) {
		string text;
		in_ >> text;
		data = 0;
		for (size_t i = 0; i < text.size(); i++)
			if (isdigit(text[i]))
				data = data * 10 + text[i] - '0';
			else
				return Error::CorruptedArchive;
		return Error::NoError;
	}
	template <class T>
	Error process(T& data) {return Error::CorruptedArchive;}
};
