#pragma once

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

    template<class T, class... Args>
    Error process(T &&val, Args &&...args)
    {
        if (process(std::forward<T>(val)) != Error::NoError) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
    }

    Error process(uint64_t val)
    {
        if (!(out_ << val << Separator)) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool val)
    {
        if (!(out_ << std::boolalpha << val << Separator)) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process()
    {
        return Error::NoError;
    }

public:
    explicit Serializer(std::ostream &out): out_(out) {}

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
};


class Deserializer
{
private:
    std::istream& in_;

    static bool check_is_uint(const std::string &s)
    {
        if (s.empty() || (!isdigit(s[0]) && s[0] != '+')) {
            return false;
        }

        size_t sz;
        stoull(s, &sz);
        return sz == s.size();
    }

    template<class T, class... Args>
    Error process(T &&val, Args &&...args)
    {
        if (process(std::forward<T>(val)) != Error::NoError) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
    }

    Error process(uint64_t &val)
    {
        std::string s;
        in_ >> s;

        if (!check_is_uint(s)) {
            return Error::CorruptedArchive;
        }

        val = std::stoull(s);
        return Error::NoError;
    }

    Error process(bool &val)
    {
        std::string s;
        in_ >> s;

        if (s != "true" && s != "false") {
            return Error::CorruptedArchive;
        }

        val = s == "true";
        return Error::NoError;
    }

    Error process()
    {
        return Error::NoError;
    }

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
};

