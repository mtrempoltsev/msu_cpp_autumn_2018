#pragma once
#include <iostream>
#include <string>


enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{

    static constexpr char Separator = ' ';
    using UInt = uint64_t;
    
    std::ostream& out_;

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args)
    {
        if (process(std::forward<T>(value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }

    template<class... ArgsT>
    Error process() {
        return Error::CorruptedArchive;
    }

    Error process(bool value) 
    {
        const char* string_v = value ? "true" : "false";

        if (out_ << string_v << Separator)
            return Error::NoError;

        return Error::CorruptedArchive;
    }

    Error process(UInt value)
    {
        if (out_ << value << Separator)
            return Error::NoError;

        return Error::CorruptedArchive;
    }

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    { }

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
    using UInt = uint64_t;

    std::istream& in_;

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args)
    {
        if (process(std::forward<T>(value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }

    template<class... ArgsT>
    Error process()
    {
        return Error::CorruptedArchive;
    }

    Error process(bool& value) 
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

    Error process(UInt& value) 
    {
        std::string text;
        in_ >> text;

        for (auto c: text) {
            if (c >= '0' && c <= '9') 
                value = value*10 + c - '0';
            else 
                return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    { }

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
};
