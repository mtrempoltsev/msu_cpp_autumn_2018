#include <iostream>
#include <sstream>
#include <cstring>

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

    template<class T>
    Error process(T &&val) {
        if (typeid(val) == typeid(bool))
            out_ << (val ? "true" : "false");
        else if (typeid(val) == typeid(u_int64_t))
            out_ << val;
        else return Error::CorruptedArchive;

        return Error::NoError;
    }

    template<class T, class... Args>
    Error process(T &&val, Args &&... args) {
        if (typeid(val) == typeid(bool))
            out_ << (val ? "true" : "false") << Separator;
        else if (typeid(val) == typeid(u_int64_t))
            out_ << val << Separator;
        else return Error::CorruptedArchive;
        process(std::forward<Args>(args)...);
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

    Error load(u_int64_t &value) {
        std::string str;
        in_ >> str;
        if ((str[0] == '-') || (str[0] == '\0'))
            return Error ::CorruptedArchive;
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

#include <iostream>
#include <sstream>

#define checkEqual(x, y) do { if ((x) != (y)) { std::cout << "at line " << __LINE__ << ": " << (x) << " != " << (y) << '\n'; }; } while(0)
#define checkTrue(cond) do { if (!(cond)) std::cout << "at line " << __LINE__ << ": " << #cond << '\n'; } while(0)

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};
