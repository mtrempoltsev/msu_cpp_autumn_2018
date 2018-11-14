#pragma once
#include <iostream>
#include <string>
using namespace std;

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    ostream& out_;
    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(forward<Args>(args)...);
    }

    Error process(uint64_t& val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }

    Error process(bool& val)
    {
        if (val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return Error::NoError;
    }

    template <class T>
    Error process(T& val)
    {
        return Error::CorruptedArchive;
    }
public:
    explicit Serializer(ostream& out)
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
};

class Deserializer
{
    static constexpr char Separator = ' ';
    istream& in_;

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(forward<Args>(args)...);
    }

    Error process(uint64_t& val)
    {
        string text;
        in_ >> text;

        if (text.size() == 0)
            return Error::CorruptedArchive;
        val = 0;
        for (size_t i = 0; i < text.size(); i++)
        {
            if (isdigit(text[i]))
                val = val * 10 + (text[i] - '0');
            else
                return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool& val)
    {
        string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    template <class T>
    Error process(T& val)
    {
        return Error::CorruptedArchive;
    }

public:
    explicit Deserializer(istream& in)
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
        return process(forward<ArgsT>(args)...);
    }
};
