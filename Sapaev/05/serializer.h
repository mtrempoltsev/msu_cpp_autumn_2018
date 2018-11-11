#pragma once

#include <cstdint>
#include <utility>
#include <iostream>
#include <string>
#include <cassert>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer
{
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
    
    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
    std::ostream& out_;
    // process использует variadic templates
    template <class... ArgsT>
    Error process(uint64_t Head, ArgsT&&... Tail)
    {
        out_ << Head << Separator;
        return process(std::forward<ArgsT>(Tail)...);
    }
    
    template <class... ArgsT>
    Error process(bool Head, ArgsT&&... Tail)
    {
        out_ << (Head ? "true" : "false")  << Separator;
        return process(std::forward<ArgsT>(Tail)...);
    }
    
    Error process(uint64_t Head)
    {
        out_ << Head << std::endl;
        return Error::NoError;
    }
    
    Error process(bool Head)
    {
        out_ << (Head ? "true" : "false")  << Separator;
        return Error::NoError;
    }
    
    template <class T, class... ArgsT>
    Error process(T, ArgsT&&...)
    {
        return Error::CorruptedArchive;
    }
    
    template <class T>
    Error process(T)
    {
        return Error::CorruptedArchive;
    }
};

class Deserializer
{
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
    
    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(args...);
    }
    
private:
    std::istream& in_;
    
    template <class... ArgsT>
    Error process(uint64_t& Head, ArgsT&&... Tail)
    {
        std::string tmp;
        in_ >> tmp;
        
        if (!isdigit(tmp[0])) {
            return Error::CorruptedArchive;
        }
        Head = std::stoull(tmp);
        return process(std::forward<ArgsT>(Tail)...);
    }
    
    template <class... ArgsT>
    Error process(bool& Head, ArgsT&&... Tail)
    {
        std::string tmp;
        in_ >> tmp;
        
        if (tmp == "true") {
            Head = true;
        } else if (tmp == "false") {
            Head = false;
        } else {
            return Error::CorruptedArchive;
        }
        return process(std::forward<ArgsT>(Tail)...);
    }
    
    Error process(uint64_t& Head)
    {
        std::string tmp;
        in_ >> tmp;
        
        if (!isdigit(tmp[0])) {
            return Error::CorruptedArchive;
        }
        Head = std::stoull(tmp);
        return Error::NoError;
    }
    
    Error process(bool& Head)
    {
        std::string tmp;
        in_ >> tmp;
        
        if (tmp == "true") {
            Head = true;
        } else if (tmp == "false") {
            Head = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};
