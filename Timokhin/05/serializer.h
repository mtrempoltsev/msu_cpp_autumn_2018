#pragma once

#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive,
    BadStream
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : m_out(out) {}

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
    std::ostream& m_out;

    template<typename T, typename...Args>
    Error process(T&& first, Args&&... args)
    {
        Error err;
        err = process(std::forward<T>(first));
        if (err == Error::NoError)
            err = process(std::forward<Args>(args)...);
        return err;
    }

    Error process(bool b)
    {
        m_out << ((b) ? "true" : "false") << Separator;
        return (m_out.good()) ? Error::NoError : Error::BadStream;
    }

    Error process(uint64_t val)
    {
       m_out << val << Separator;
       return (m_out.good()) ? Error::NoError : Error::BadStream;
    }

};

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : m_in(in) {}

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    std::istream& m_in;

    template<typename T, typename...Args>
    Error process(T&& first, Args&&... args)
    {
        Error err;
        err = process(std::forward<T>(first));
        if (err == Error::NoError)
            err = process(std::forward<Args>(args)...);
        return err;
    }

    Error process(bool& b)
    {
        std::string tmp;
        m_in >> tmp;
        if (!m_in.good())
            return Error::BadStream;
        if (tmp == "true")
            b = true;
        else if (tmp == "false")
            b = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    Error process(uint64_t& val)
    {
       std::string tmp;
       m_in >> tmp;
       if (tmp[0] == '-')
       {
           return Error::CorruptedArchive;
       }
       else
       {
           try{
               val = std::stoull(tmp);
           }catch(...){
               return Error::CorruptedArchive;
           }
       }
       return Error::NoError;
    }

};
