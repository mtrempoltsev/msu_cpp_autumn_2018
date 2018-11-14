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
    Error c_out(uint64_t a) {
        out_ << a;
        return Error::NoError;
    }

    Error c_out(bool b) {
        out_ << (b ? "true" : "false");
        return Error::NoError;
    }

    template <class T>
    Error c_out(T var) {
        return Error::CorruptedArchive;
    }

    template <class T>
    Error process(T var) {
        return c_out(var);
    }

    template<class T, class... ArgsT>
    Error process(T var, ArgsT&&... args) {
        if (c_out(var) == Error::NoError) {
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
    Error operator()(ArgsT&&... args) {
        return process(args...);
    }

private:
    Error c_in(bool& b) {
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

    Error c_in(uint64_t& a) {
        std::string s;
        in_ >> s;
        try {
            if (s[0] == '-') {
                throw std::invalid_argument("");
            }
            a = std::stoull(s.c_str());
        } catch (std::exception& exc) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template<class T>
    Error c_in(T& var) {
        return Error::CorruptedArchive;
    }

    template<class T>
    Error process(T& var) {
        return c_in(var);
    }

    template<class T, class... ArgsT>
    Error process(T& var, ArgsT&&... args) {
        if (process(var) == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        }
        return Error::CorruptedArchive;
    }
};
