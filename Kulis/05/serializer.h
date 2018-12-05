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
public:
    explicit Serializer(std::ostream& out)
        : out(out)
    {
    }

    template <class T>
    Error save(T & object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args)
    {
        return process(std::forward<Args>(args)...);
    }

private:
    template <class Arg, class ... Args>
    Error process(Arg && arg, Args &&... args)
    {
        return process(std::forward<Args>(args)...) == Error::NoError ?
            process(std::forward<Args>(args)...) : Error::CorruptedArchive;
    }

    template <class T>
    Error process(T &&)
    {
        return Error::CorruptedArchive;
    }

    Error process(uint64_t data)
    {
        out << data << sep;
        return Error::NoError;
    }

    Error process(bool data)
    {
        out << (data ? "true" : "false") << sep;
        return Error::NoError;
    }

    static constexpr char sep = ' ';
    std::ostream& out;
};

class Deserializer
{
public:
    explicit Deserializer(std::istream & in)
        : in(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    template <class Arg, class ... Args>
    Error process(Arg & arg, Args &&... args)
    {
        return process(std::forward<Args>(args)...) == Error::NoError ?
            process(std::forward<Args>(args)...) : Error::CorruptedArchive;
    }

    template <class T>
    Error process(T &)
    {
        return Error::CorruptedArchive;
    }

    Error process(uint64_t & data)
    {
        std::string s;
        in >> s;

        if (!isdigit(s[0])) {
            return Error::CorruptedArchive;
        }
        data = std::stoull(s);
        return Error::NoError;
    }

    Error process(bool & data)
    {
        std::string s;
        in >> s;
        if ((s != "false") && (s != "true")) {
            return Error::CorruptedArchive;
        }
        data = (s == "true");
        return Error::NoError;
    }

    std::istream & in;
};
