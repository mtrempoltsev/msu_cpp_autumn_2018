#pragma once

#include <iostream>
#include <string>

enum class Error {
    NoError,
    CorruptedArchive
};


class Serializer {
private:
    static constexpr char Separator = ' ';
    std::ostream& out_;
public:
    explicit Serializer(std::ostream& out)
        : out_ (out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(args...);
    }
    
private:
    // process использует variadic templates
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        if (process(std::forward<T>(value)) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    Error process(bool& value) {
        if (value) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return Error::NoError;
    }

    Error process(uint64_t& value) {
        out_ << value << Separator;
        return Error::NoError;
    }

    template <class T>
    Error process(T& value) {
        return Error::CorruptedArchive;
    }

};


class Deserializer {
private:
    std::istream& in_;
public:
    explicit Deserializer(std::istream& in)
        : in_ (in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(args...);
    }

private:
    // process использует variadic templates
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        if (process(std::forward<T>(value)) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(args)...);
    }

    Error process(bool& value) {
        std::string text;
        in_ >> text;

        if (text == "true") {
            value = true;
        }
        else if (text == "false") {
            value = false;
        }
        else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error process(uint64_t& value) {
        std::string text;
        in_ >> text;
        try {
            value = std::stoull(text);
            if (std::to_string(value) != text) {
                throw std::invalid_argument("");
            }
        } catch (const std::exception& ex) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T>
    Error process(T& value) {
        return Error::CorruptedArchive;
    }
};