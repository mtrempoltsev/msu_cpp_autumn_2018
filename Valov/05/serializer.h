#pragma once
#include <string>

enum class Error   
{
	NoError,
	CorruptedArchive
};                 

class Serializer
{
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
	static constexpr char Separator = ' ';
	std::ostream& out_;
	
	Error process(uint64_t x)
	{
		out_ << x << Separator;
		return Error::NoError;
	}

	Error process(bool x)
	{
		if (x)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;
		return Error::NoError;
	}
	
	template <class T>
	Error process(T x)
	{
		return Error::CorruptedArchive;
	}

	template <class T, class... Args>
	Error process(T&& x, Args&&... args)
	{
		if (process(x) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		else
			return process(std::forward<Args>(args)...);
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

	Error load(uint64_t& x) 
	{
		std::string str;
		in_ >> str;
		if (str.empty()) 
		{
			return Error::CorruptedArchive;
		}
		else if (str[0] == '-') 
		{
			return Error::CorruptedArchive;
		}
		else
		{
			try 
			{
				x = stoul(str);
			}
			catch (...) 
			{
				return Error::CorruptedArchive;
			}
		}
		return Error::NoError;
	}

	Error load(bool& x)
	{
		std::string text;
		in_ >> text;

		if (text == "true")
			x = true;
		else if (text == "false")
			x = false;
		else
			return Error::CorruptedArchive;
		return Error::NoError;
	}

	template <class T>
	Error process(T& x) 
	{
		return load(x);
	}

	template <class T, class... Args>
	Error process(T&& x, Args &&... args) 
	{
		if (load(x) == Error::CorruptedArchive) 
		{
			return Error::CorruptedArchive;
		}
		return process(std::forward<Args>(args)...);
	}

};
