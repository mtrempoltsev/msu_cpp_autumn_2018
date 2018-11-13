#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;
    
    template <class T, class... Args>
	Error process(T&& value, Args&&... args)
	{
		if (process(value) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<Args>(args)...);
		}
	}
	
	Error process (uint64_t& value)
	{
		out_ << value << Separator;
		return Error::NoError;
	}
	
	Error process (bool& value)
	{
		if (value)
		{
			out_ << "true" << Separator;
		}
		else
		{
			out_ << "false" << Separator;
		}
		return Error::NoError;
	}
    
public:

    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T&& object)
    {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(args...);
    }
};

class Deserializer
{
    std::istream& in_;
    
    template <class T, class... Args>
	Error process(T&& value, Args&&... args)
	{
		if (process(value) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<Args>(args)...);
		}	
	}

	Error process(uint64_t& value)
	{
		std::string text;
		in_ >> text;
		
		if (text == "")
		{
			return Error::CorruptedArchive;
		}
		else
		{
			for (auto c: text)
			{
				if ((c > 57) || (c < 48))
				{
					return Error::CorruptedArchive;
				}
			}
			value = std::stoull(text);
			
			return Error::NoError; 		
		}
	}

	Error process(bool& value)
	{
		std::string text;
		in_ >> text;
		
		if (text == "true")
		{
			value = true;
		}
		else
		{
			if (text == "false")
			{
				value = false;
			}
			else
			{
				return Error::CorruptedArchive;
			}
		}
		return Error::NoError;
	}
	
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }
    
    template <class T>
    Error load(T&& object)
    {
		return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(args...);
    }
};
