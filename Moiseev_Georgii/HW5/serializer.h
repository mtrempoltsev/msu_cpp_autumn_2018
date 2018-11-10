#pragma once

#include <cstdint>
#include <utility>
#include <iostream>
#include <string>
#include <cassert>

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
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
    std::ostream& out_;

    template <class... ArgsT>
    Error process(uint64_t x, ArgsT&&... args)
    {
        out_ << x << Separator;
        return process(std::forward<ArgsT>(args)...);
    }

    template <class... ArgsT>
    Error process(bool x, ArgsT&&... args)
    {
        if (x == true)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return process(std::forward<ArgsT>(args)...);
    }

    template <class T, class... ArgsT>
    Error process(T x, ArgsT&&... args)
    {
        return Error::CorruptedArchive;
    }

    Error process(uint64_t x)
    {
        out_ << x;
        return Error::NoError;
    }

    Error process(bool x)
    {
        if (x == true)
            out_ << "true";
        else
            out_ << "false";
        return Error::NoError;
    }

    template <class T>
    Error process(T x)
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

    template <class... ArgsT>
    Error process(uint64_t& x, ArgsT&&... args)
    {
        static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "ull not large enough");

        std::string term;
        in_ >> term;

        try
        {
            if (term[0] == '-')
                return Error::CorruptedArchive;

            x = std::stoull(term);
        }
        catch (std::exception& e)
        {
            return Error::CorruptedArchive;
        }

        return process(std::forward<ArgsT>(args)...);
    }

    template <class... ArgsT>
    Error process(bool& x, ArgsT&&... args)
    {
        std::string term;
        in_ >> term;

        if (term == "true")
            x = true;
        else if (term == "false")
            x = false;
        else
            return Error::CorruptedArchive;
        return process(std::forward<ArgsT>(args)...);
    }

    Error process(uint64_t& x)
    {
        static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "ull not large enough");

        std::string term;
        in_ >> term;

        try
        {
            if (term[0] == '-')
                return Error::CorruptedArchive;

            x = std::stoull(term);
        }
        catch (std::exception& e)
        {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error process(bool& x)
    {
        std::string term;
        in_ >> term;

        if (term == "true")
            x = true;
        else if (term == "false")
            x = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }
};