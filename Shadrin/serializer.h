#pragma once

#include <iostream>
#include <type_traits>
#include <sstream>
#include <exception>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{

    static constexpr char Separator = ' ';

    std::ostream &out; 

    template <class T>
    inline bool _gen_process(T&& val) {
        if constexpr (std::is_same_v<T, bool>) {
            out << (val ? "true" : "false");
        } else if constexpr (std::is_same<T, uint64_t>::value) {
            out << val;
        } else {
            return false;
        }
        out << Separator;
        return true;
    }

    template <class... Args>
    Error process(Args&&... args) {
        return ((_gen_process(std::forward<Args>(args)) && ...)
            ? Error::NoError
            : Error::CorruptedArchive);
    }

public:

    explicit Serializer(std::ostream& out_)
        : out(out_)
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
        return process(std::forward<ArgsT>(args)...);
    }

};

class Deserializer {

    std::istream &in; 

    template <class T>
    inline bool _gen_process(T&& val) {
        std::string s;
        in >> s;
        if constexpr (std::is_same_v<T, bool>) {
            if (s == "true") {
                val = true;
            } else if (s == "false") {
                val = false;
            } else {
                return false;
            }
        } else if constexpr (std::is_same_v<T, uint64_t>) {
            if (s[0] == '-') {
                return false;
            } else {
                uint64_t t;
                std::size_t pos;
                try {
                    t = std::stoull(s, &pos);
                } catch (const std::invalid_argument &e) {
                    return false;
                } catch (const std::out_of_range &e) {
                    return false;
                }
                if (pos != s.size()) {
                    return false;
                }
                val = t;
            }
        } else {
            return false;
        }
        return true;
    }

    template <class... Args>
    Error process(Args&&... args) {
        return ((_gen_process(std::forward<Args>(args)) && ...)
                ? Error::NoError
                : Error::CorruptedArchive);
    }

public:

    explicit Deserializer(std::istream &in_)
        : in(in_)
    {
    }

    template <class T>
    Error load(T &object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

};
