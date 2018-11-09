#pragma once

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
    explicit Serializer(std::ostream& out) : out_(out) {}

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
	template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) 
    {
        if (process(value) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<ArgsT&>(args)...);
		}
	}

    Error process(bool& value)
    {
    	try
    	{
    		out_ << ((value == true)?("true"):("false")) << Separator;
    	}
		catch(...)
		{
			return Error::CorruptedArchive;
		}	
		return Error::NoError;
    }
    Error process(uint64_t& value)
    {
        try
        {
        	out_ << value << Separator;
        }
        catch(...)
		{
			return Error::CorruptedArchive;
		}	
		return Error::NoError;
    }
};



class Deserializer
{
    std::istream& in_;
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

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
    template <class T, class... ArgsT>
    Error process(T& value, ArgsT&... args) 
    {
        if (process(value) == Error::CorruptedArchive)
		{
			return Error::CorruptedArchive;
		}
		else
		{
			return process(std::forward<ArgsT&>(args)...);
		}	
    }

    Error process(bool& value)
    {
        std::string str;
        in_ >> str;
        if(str == "true")
        {
            value = true;
        }
        else if (str == "false")
        {
            value = false;
        } 
        else return Error::CorruptedArchive;
        return Error::NoError;
    }

    Error process(uint64_t& value)
    {
        std::string str;
        in_>> str;
        if (str == "")
		{
			return Error::CorruptedArchive;
		}
		else
		{
			for (auto s : str) 
			{
				if (!isdigit(s)) 
				{
					return Error::CorruptedArchive;
        		}
    		}
    		value = std::stoull(str);
			return Error::NoError;
		}
    }
};