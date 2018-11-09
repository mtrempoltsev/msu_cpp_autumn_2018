#pragma once

#include <iostream>
#include <string>
#include <cstring>

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

    Error save(bool val) {
    	out_ << (val ? "true" : "false");
    	return Error::NoError;
    }

    Error save(uint64_t val) {
    	out_ << val;
    	return Error::NoError;
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
	std::ostream& out_;

	template <class T, class... ArgsT>
    Error process(T val, ArgsT... args) {
    	Error err = save(val);
    	if (err != Error::NoError) {
    		return err;
    	}
    	out_ << Separator;
    	return process(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error process(T val) {
    	return save(val);
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

	Error load(bool& value)
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

	Error load(uint64_t& value)
	{
	    std::string text;
	    in_ >> text;

	    uint64_t res = 0;
	    for (auto c : text) {
	    	if (!isdigit(c)) {
	    		return Error::CorruptedArchive;
	    	}
	    	res = (res * 10) + (c - '0');
	    }

	    value = res;

	    return Error::NoError;
	}

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
    
private:
	std::istream& in_;

	template <class T, class... ArgsT>
    Error process(T& val, ArgsT&... args) {
    	Error err = load(val);
    	if (err != Error::NoError) {
    		return err;
    	}
    	return process(std::forward<ArgsT&>(args)...);
    }

    template <class T>
    Error process(T& val) {
    	return load(val);
    }
};
