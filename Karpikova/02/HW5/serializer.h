#include <string>
#include <sstream>
#pragma once
using namespace std;

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
private:
    ostream& out_;
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
    Error process(uint64_t value)
    {
        out_ << value << Separator;
        return Error::NoError;
    }

    Error process(bool value)
    {
        if (value) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return Error::NoError;
    }

    template <class T>
    Error process(T& value)
    {
        return Error::CorruptedArchive;
    }

    template <class T, class... ArgsT>
    Error process(T& value, ArgsT&... args)
    {
        if (process(value) == Error::NoError) {
		return process(args...);
	}
	return Error::CorruptedArchive;
    }
};


class Deserializer
{
private:
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

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
private:
    Error process(uint64_t& value)
	{
        string str;
        in_ >> str;
        int i = 0; 
        while (isdigit(str[i])) ++i;
        if (!isdigit(str[i - 1])) return Error::CorruptedArchive;
        value = stoull(str);
        return Error::NoError;
    }
    Error process(bool& value)
    {
        string txt;
        in_ >> txt;
        if (txt == "false") {
            value = false;
        } else if (txt == "true") {
            value = true;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T>
    Error process(T& value)
    {
        return Error::CorruptedArchive;
    }

    template <class T, class... ArgsT>
    Error process(T& value, ArgsT&... args)
    {
        if (process(value) == Error::NoError) {
		return process(args...);
	}
	return Error::CorruptedArchive;
    }

};
