#pragma once

#include <iostream>
#include <type_traits>
#include <sstream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
private:
    static constexpr char Separator = ' ';
    std::ostream& Out;

    Error process(bool val) {
        Out << (val ? "true" : "false");
        return Error::NoError;
    }

    Error process(uint64_t val) {
        Out << val;
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T val, Args&&... args) {
        if (process(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        Out << Separator;
        return process(std::forward<Args>(args)...);
    }

public:
    explicit Serializer(std::ostream& out)
        : Out(out)
    {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args... args) {
        return process(args...);
    }
};

class Deserializer {
private:
    std::istream& In;

    Error process(bool& val) {
        std::string s;
        In >> s;
        if (s == "true") {
            val = true;
        } else if (s == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(uint64_t& val) {
        std::string s;
        In >> s;
        if (s.front() == '-') {
            return Error::CorruptedArchive;
        }
        size_t pos;
        uint64_t t;
        try {
            t = std::stoull(s, &pos);
        } catch (std::exception& /*e*/) {
            return Error::CorruptedArchive;
        }
        if (pos != s.size()) {
            return Error::CorruptedArchive;
        }
        val = t;
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T& val, Args&&... args) {
        if (process(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
    }

public:
    explicit Deserializer(std::istream& in)
        : In(in)
    {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&... args) {
        return process(args...);
    }
};
