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

private:
    std::ostream& out_;

    template <class T, class... ArgsT>
    Error process(T&& first, ArgsT&&... rest)
    {
        if(process(std::forward<T>(first)) != Error::NoError) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(rest)...);
    }

    Error process(uint64_t val)
    {
        auto& succ = (out_ << val << Separator);
        return succ ? Error::NoError : Error::CorruptedArchive;
    }

    Error process(bool val)
    {
        auto& succ = (out_ << (val ? "true" : "false") << Separator);
        return succ ? Error::NoError : Error::CorruptedArchive;
    }

    Error process() {
        return Error::NoError;
    }

    template <class T>
    Error process(T& val)
    {
        return Error::CorruptedArchive;
    }
};

class Deserializer
{
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

private:
    std::istream& in_;

    template <class T, class... ArgsT>
    Error process(T&& first, ArgsT&&... rest)
    {
        if(process(std::forward<T>(first)) != Error::NoError) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(rest)...);
    }

    Error process(uint64_t& val)
    {
        std::string s;
        in_ >> s;
        if(s[0] == '-') {
            return Error::CorruptedArchive;
        }
        try {
            val = std::stoull(s);
        }
        catch(std::invalid_argument&) {
            return Error::CorruptedArchive;
        }
        catch(std::out_of_range&) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool& val)
    {
        std::string s;
        in_ >> s;
        if(s == "true") {
            val = true;
        } else if(s == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process() {
        return Error::NoError;
    }

    template <class T>
    Error process(T& val)
    {
        return Error::CorruptedArchive;
    }
};
