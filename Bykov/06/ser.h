#include <iostream>
#include <string>
#include <iomanip>


enum class Error
{
    NoError,
    CorruptedArchive
};


class Serializer
{
private:
    static constexpr char Separator = ' ';
    std::ostream &out_;
public:
    explicit Serializer(std::ostream &out): out_(out)
    {
    }

    template<class T>
    Error save(T &object)
    {
        return object.serialize(*this);
    }

    template<class... Args>
    Error operator()(Args &&...args)
    {
        return process(std::forward<Args>(args)...);
    }
private:
    template<class T, class... Args>
    Error process(T &&val, Args &&...args)
    {
        bool b = write(val) == Error::NoError;
        bool b1 = process(std::forward<Args>(args)...) == Error::NoError;
        if (b && b1)
            return Error::NoError;
        else
            return Error::CorruptedArchive;
    }

    Error write(uint64_t val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }

    Error write(bool val)
    {
        std::string strVal = "false";
        if (val)
            strVal = "true";
        out_ << strVal << Separator;
        return Error::NoError;
    }

    Error write(...)
    {
        return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }
};

class Deserializer
{
    std::istream& in_;
public:
    explicit Deserializer(std::istream& in): in_(in) {}

    template<class T>
    Error load(T &object)
    {
        return object.serialize(*this);
    }

    template<class... Args>
    Error operator()(Args &&...args)
    {
        return process(std::forward<Args>(args)...);
    }
private:
    template<class T, class... Args>
    Error process(T &&val, Args &&...args)
    {
        bool b = load(val) == Error::NoError;
        bool b1 = process(std::forward<Args>(args)...) == Error::NoError;
        if (b && b1)
            return Error::NoError;
        return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
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
        bool err = false;
        std::string s;
        in_ >> s;
        err = s[0] == '-';
        try
        {
            value = std::stoull(s);
        }
        catch(...)
        {
            err = true;
        }
        if (err)
        {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }


    Error load(...)
    {
        return Error::CorruptedArchive;
    }
};
