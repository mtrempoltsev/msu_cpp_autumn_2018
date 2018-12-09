#pragma once
#include <string>
#include <iostream>
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
    Error process(const T& curr_val, Args&&... args)
    {
        if (process(curr_val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return process(std::forward<Args>(args)...);
    }

    template <class T>
    Error process(T curr_val)
    {
        return Error::CorruptedArchive;
    }

    Error process(uint64_t curr_val)
    {
        out_ << curr_val << Separator;
        return Error::NoError;
    }

    Error process(bool curr_val)
    {
        if (curr_val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return Error::NoError;
    }

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
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
};

class Deserializer
{
    std::istream& in_;

    template <class T, class... Args>
    Error process(T& curr_val, Args&&... args)
    {
        if (process(curr_val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return process(std::forward<Args>(args)...);
    }

    template <class T>
    Error process(T curr_val)
    {
        return Error::CorruptedArchive;
    }

    Error process(uint64_t& curr_val)
    {
        std::string text;
        in_ >> text;

        if (text.size() == 0)
            return Error::CorruptedArchive;
        curr_val = 0;
        for (size_t i = 0; i < text.size(); i++)
        {
            if (isdigit(text[i]))
                curr_val = curr_val * 10 + (text[i] - '0');
            else
                return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool& curr_val)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            curr_val = true;
        else if (text == "false")
            curr_val = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

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
        return process(std::forward<ArgsT>(args)...);
    }
};
