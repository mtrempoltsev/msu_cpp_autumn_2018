#pragma once

#include <iostream>
#include <string>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T&& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {

        return process(std::forward<ArgsT>(args)...);
    }

    Error save(bool value) {
        out_ << (value ? "true" : "false");
        return Error::NoError;
    }

    Error save(uint64_t value) {
        out_ << value;
        return Error::NoError;
    }
    
private:
    std::ostream& out_;
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        save(std::forward<T&>(value));
        out_ << Separator;
        return process(std::forward<ArgsT&>(args)...);
    }
   
    template <class T>
    Error process(T&& value) {
        return save(std::forward<T>(value));
    }
};


class Deserializer {
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(std::forward<ArgsT&>(args)...);
    }

    Error load(bool& value) {
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

    Error load(uint64_t& value) {
        std::string text;
        in_ >> text;
        value = 0;
        for (auto i : text) {
            if (isdigit(i)) {
                value = value * 10 + i - '0';
            } else {
                return Error::CorruptedArchive;
            } 
        }  
        return Error::NoError;
    } 

private:
    std::istream& in_;
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args) {
        if (load(std::forward<T&>(value)) != Error::NoError) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT&>(args)...);
    }

    template <class T>
    Error process(T&& value) {
        return load(std::forward<T&>(value));
    }
};


