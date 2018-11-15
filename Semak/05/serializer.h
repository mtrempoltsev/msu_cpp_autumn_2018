#pragma once

#include <sstream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out)
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

    template <class T, class... ArgsT>
    Error process(T&& val, ArgsT&&... args)
    {
        Error er = process(val);
        if(er == Error::NoError)
            return process(std::forward<ArgsT>(args)...);
        else
            return Error::CorruptedArchive;
    }

    Error process(uint64_t val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }

    Error process(bool val)
    {
        out_ << (val? "true" : "else") << Separator;
        return Error::NoError;
    }

    Error process()
    {
        return Error::NoError;
    }
};

class Deserializer
{
public:
    explicit Deserializer(std::istream& in) : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT> Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    std::istream& in_;


    template <class T, class... ArgsT>
    Error process(T& val, ArgsT&&... args)
    {
        Error er = process(val);
        if(er == Error::NoError)
            return process(std::forward<ArgsT>(args)...);
        else
            return Error::CorruptedArchive;
    }

    Error process(uint64_t& val)
    {
        std::string strVal;
        in_ >> strVal;
        try
        {
            if(strVal[0] == '-')
                throw std::invalid_argument("Negative number");
            val = std::stoull(strVal);
        }
        catch(std::exception& ex)
        {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool& val)
    {
        std::string strVal;
        in_ >> strVal;
        if(strVal == "true")
            val = true;
        else if(strVal == "false")
            val = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error process()
    {
        return Error::NoError;
    }
};