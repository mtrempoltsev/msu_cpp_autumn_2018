#pragma once
#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
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

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }

private:
    std::ostream& out_;
    Error process(bool val)
    {
        out_ << (val ? "true" : "false");
        return Error::NoError;
    }

    Error process(uint64_t val)
    {
        out_ << val;
        return Error::NoError;
    }
    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (process(std::forward<T>(val)) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        out_ << Separator;
        return process(std::forward<Args>(args)...);
    }
};

class Deserializer {
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

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }

private:
    std::istream& in_;

    Error process(bool& val)
    {
        std::string s;
        in_ >> s;
        if (s == "true") {
            val = true;
        } else if (s == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(uint64_t& val)
    {
        std::string s;
        in_ >> s;
        if (s.front() == '-') {
            return Error::CorruptedArchive;
        }
        size_t pos;
        uint64_t t;
        try {
            t = std::stoull(s, &pos);
        } catch (std::exception e) {
            return Error::CorruptedArchive;
        }
        if (pos != s.size()) {
            return Error::CorruptedArchive;
        }
        val = t;
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        if (process(std::forward<T>(val)) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
    }
};
