#pragma once

#include <iostream>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    std::ostream& out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    Error process(uint64_t& a) {
        out_ << a;
        return Error::NoError;
    }

    Error process(bool& b) {
        out_ << (b ? "true" : "false");
        return Error::NoError;
    }

    template <class T>
    Error process(T&& var) {
        return Error::CorruptedArchive;
    }

    template<class T, class... ArgsT>
    Error process(T&& var, ArgsT&&... args) {
        if (process(var) == Error::NoError) {
            out_ << Separator;
            return process(std::forward<ArgsT>(args)...);
        }
        return Error::CorruptedArchive;
    }
};

class Deserializer {
    std::istream& in_;
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }

private:
    Error process(bool& b) {
        std::string s;
        in_ >> s;
        if (s == "true")
            b = true;
        else if (s == "false")
            b = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    Error process(uint64_t& a) {
        std::string s;
        in_ >> s;
        if (s[0] == '-') {
            return Error::CorruptedArchive;
        }
        try {
            a = std::stoull(s.c_str());
        } catch (std::invalid_argument) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template<class T>
    Error process(T& var) {
        return Error::CorruptedArchive;
    }

    template<class T, class... ArgsT>
    Error process(T&& var, ArgsT&&... args) {
        if (process(var) == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        }
        return Error::CorruptedArchive;
    }
};
