#pragma once

#include <iostream>
#include <type_traits>
#include <sstream>

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
    inline Error _gen_process(T&& val) {
        if (std::is_same<T, bool &>::value) {
            out << (val ? "true" : "false");
        } else if (std::is_same<T, uint64_t &>::value) {
            out << val;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T>
    Error process(T&& val) {
        return _gen_process(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        if (_gen_process(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        out << Separator;
        return process(std::forward<Args>(args)...);
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
        return process(args...);
    }

};

class Deserializer {

    std::istream &in; 

    template <class T>
    inline Error _gen_process(T&& val) {
        std::string s;
        in >> s;
        if (std::is_same<T, bool &>::value) {
            if (s == "true") {
                val = true;
            } else if (s == "false") {
                val = false;
            } else {
                return Error::CorruptedArchive;
            }
        } else if (std::is_same<T, uint64_t &>::value) {
            if (s[0] == '-') {
                return Error::CorruptedArchive;
            } else {
                uint64_t t;
                std::size_t pos;
                try {
                    t = std::stoull(s, &pos);
                } catch (...) {
                    return Error::CorruptedArchive;
                }
                if (pos != s.size()) {
                    return Error::CorruptedArchive;
                }
                val = t;
            }
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T>
    Error process(T&& val) {
        return _gen_process(val);
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        if (_gen_process(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
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
        return process(args...);
    }

};
