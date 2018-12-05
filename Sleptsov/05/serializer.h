#pragma once

#include <iostream>
#include <string>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream& out;

    template <class T>
    Error write(T& val)
    {
        if (out << val << Separator) {
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }

    template <class T>
    Error process(T&& val) {
        return process_handler(std::forward<T>(val));
    }

    template <class T, class... ArgsT>
    Error process(T&& val, ArgsT&&... args) 
    {
        if (process_handler(std::forward<T>(val)) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    Error process_handler(bool val) 
    {
        std::string str(val ? "true" : "false");
        return write(str);
    }

    Error process_handler(uint64_t val) 
    {
        return write(val);
    }

public:
    explicit Serializer(std::ostream& out)
        : out(out)
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


class Deserializer {
    std::istream& in;

    template <class T>
    Error process(T&& val) {
        return process_handler(std::forward<T>(val));
    }

    template <class T, class... ArgsT>
    Error process(T&& val, ArgsT&&... args) 
    {
        if (process_handler(std::forward<T>(val)) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    Error process_handler(bool& val) 
    {
        std::string str;
        in >> str;

        if (str == "true") {
            val = true;
        } else if (str == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process_handler(uint64_t& val) 
    {
        std::string str;
        in >> str;
        
        uint64_t tmp = 0;
        for (auto c: str) {
            if (std::isdigit(c)) {
                tmp = tmp * 10 + c - '0';
            } else {
                return Error::CorruptedArchive;
            }
        }
        val = tmp;

        return Error::NoError;
    }

public:
    explicit Deserializer(std::istream& in)
        : in(in)
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
