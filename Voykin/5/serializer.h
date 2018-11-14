#include<iostream>
using namespace std;

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
public:
    explicit Serializer(ostream& out): out_(out){}
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
    ostream& out_;
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args)
    {
        if(process(value) == Error::NoError)
        {
            return process(forward<ArgsT>(args)...);
        }
        return Error::CorruptedArchive;
    }
    Error process()
    {
        return Error::NoError;
    }
    Error process(bool value)
    {
        if(value)
        {
            out_<<"true"<<Separator;
        }
        else
        {
            out_<<"false"<<Separator;
        }
        return Error::NoError;
    }
    Error process(uint64_t value)
    {
        out_<<value<<Separator;
        return Error::NoError;
    }
};

class Deserializer
{
public:
    explicit Deserializer(istream& in): in_(in){}
    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }
    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(forward<ArgsT>(args)...);
    }
private:
    istream& in_;
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args)
    {
        if(process(value) == Error::NoError)
        {
            return process(forward<ArgsT>(args)...);
        }
        return Error::CorruptedArchive;
    }
    Error process()
    {
        return Error::NoError;
    }
    Error process(bool& value)
    {
        string text;
        in_ >> text;
        if (text == "true")
        {
            value = true;
        }
        else if (text == "false")
        {
            value = false;
        }
        else
        {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
    Error process(uint64_t& value)
    {
        string str;
        in_ >> str;
        uint64_t a = 0;
        if((str[0] == '-') || (str[0] == '0'))
        {
            return Error::CorruptedArchive;
        }
        for (auto i: str)
        {
            if (isdigit(i))
            {
                a = a * 10 + i - '0';
            }
            else
            {
                return Error::CorruptedArchive;
            }
        }
        value = a;
        return Error::NoError;
    }
};
