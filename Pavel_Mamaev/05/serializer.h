//
//  main.cpp
//  template-sem
//
//  Created by Мамаев Павел on 08/11/2018.
//  Copyright © 2018 Мамаев Павел. All rights reserved.
//


#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
using namespace std;

enum class Error
{
    NoError,
    CorruptedArchive
};
struct Data {
    uint64_t a;
    bool b;
    uint64_t c;
    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }
};

class Serializer {
private:
    ostream& out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer (ostream& out) : out_(out) {
    }
    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }
    Error process(bool value) {
        if (value) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return Error::NoError;
    }
    Error process(uint64_t value) {
        out_ << value << Separator;
        return Error::NoError;
    }
    template <class T>
    Error process(T&& value) {
        return Error::CorruptedArchive;
    }
    template <class T, class... Args>
    Error process(T&& value, Args&&... args) {
        Error tmp = process(value);
        if (tmp == Error::NoError) {
            return process(forward<Args>(args)...);
        }
        return tmp;
    }
    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(forward<ArgsT>(args)...);
    }
    
    
};
class Deserializer {
private:
    std::istream& in_;
public:
    explicit Deserializer (std::istream& in) : in_(in) {
    }
    Error process(bool& value) {
        string text;
        in_ >> text;
        if (text == "true") {
            value = true;
        } else if (text == "false") {
            value = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
    Error process(uint64_t& value){
        string text;
        in_ >> text;
        value = 0;
        for (auto el : text) {
            if (el >= '0' && el <= '9') {
                value = value * 10 + el - '0';
            } else {
                return Error::CorruptedArchive;
            }
        }
        return Error::NoError;
    }
    template <class T>
    Error process(T& value) {
        return Error::CorruptedArchive;
    }
    template <class T, class... Args>
    Error process(T&& value, Args&&... args) {
        Error tmp = process(value);
        if (tmp == Error::NoError) {
            return process(forward<Args>(args)...);
        }
        return tmp;
    }
    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }
    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(forward<ArgsT>(args)...);
    }
};
