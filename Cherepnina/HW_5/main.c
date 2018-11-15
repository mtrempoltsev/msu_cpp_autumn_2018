#include <iostream>

#pragma once

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream &out) : out_(out) {}

    template<class T>
    Error save(T &object) {
        return object.serialize(*this);
    }

    template<class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    std::ostream &out_;

    Error out_object(uint64_t &val) {
        out_ << unsigned(val) << Separator;
        return Error::NoError;
    }

    Error out_object(bool &val) {
        out_ << (val ? "true" : "false") << Separator;
        return Error::NoError;
    }

    template<class T>
    Error out_object(T &val) {
        return Error::CorruptedArchive;
    }

    template<class T>
    Error process(T &&val) {
        return out_object(val);
    }

    template<class T, class... Args>
    Error process(T &&val, Args &&... args) {
        if (out_object(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }

};

class Deserializer {
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream &in) : in_(in) {}

    template<class T>
    Error load(T &object) {
        return object.serialize(*this);
    }

    template<class... ArgsT>
    Error operator()(ArgsT &&... args) {
        return process(args...);
    }

private:
    std::istream &in_;

    Error load(uint64_t &value) {
        std::string str;
        in_ >> str;
        if ((str[0] == '-') || (str[0] == '\0'))
            return Error::CorruptedArchive;
        value = stoul(str);
        return Error::NoError;
    }

    Error load(bool &value) {
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    template<class T>
    Error process(T &val) {
        return load(val);
    }

    template<class T, class... Args>
    Error process(T &&val, Args &&... args) {
        if (load(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }
};
