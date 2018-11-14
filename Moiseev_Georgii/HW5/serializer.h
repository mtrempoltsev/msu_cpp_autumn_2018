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

    template <class T, class... ArgsT>
    Error process(T x, ArgsT&&... args)
    {
        write(x);
        out_ << Separator;
        return process(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error process(T x)
    {
        write(x);
        return Error::NoError;
    }

    void write(bool x)
    {
        if (x == true)
            out_ << "true";
        else
            out_ << "false";
    }

    void write(uint64_t x)
    {
        out_ << x;
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

    template <class T, class... ArgsT>
    Error process(T& x, ArgsT&&... args)
    {
        if (read(x) == false)
            return Error::CorruptedArchive;

        return process(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error process(T& x)
    {
        if (read(x) == false)
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    bool read(bool& x)
    {
        std::string term;
        in_ >> term;

        if (term == "true")
        {
            x = true;
            return true;
        }
        else if (term == "false")
        {
            x = false;
            return true;
        }
        else
            return false;
    }

    bool read(uint64_t& x)
    {
        static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "ull not large enough");

        std::string term;
        in_ >> term;

        try
        {
            if (term[0] == '-')
                return false;

            x = std::stoull(term);
        }
        catch (std::exception& e)
        {
            return false;
        }

        return true;
    }
};