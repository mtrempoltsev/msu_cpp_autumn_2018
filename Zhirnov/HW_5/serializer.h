#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer {

    static constexpr char Separator = ' ';
    
    std::ostream& out_;

public:

    explicit Serializer(std::ostream& out)
        : out_(out) {}

    template <class T> 
    Error save(T& object) { return object.serialize(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT... args) { return process(args...); }


private:
    
    template <class T>
    Error process(T&& pos)
    {
        return Error::CorruptedArchive;
    }
    
    Error process(bool& pos)
    {
        if (pos)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        
        return Error::NoError;
    }
    
    Error process(uint64_t& pos)
    {
        out_ << pos << Separator;

        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& pos, Args&&... args)
    {
        if (process(pos) == Error::NoError)

            return process(std::forward<Args>(args)...);
        
        else
        
            return Error::CorruptedArchive;
    }

};

class Deserializer {

    static constexpr char Separator = ' ';
    
    std::istream& in_;

public:

    explicit Deserializer(std::istream& in_)
        : in_(in_) {}

    template <class T> 
    Error load(T& object) { return object.serialize(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) { return process(args...); }
    //необходимо передавать аргумент по ссылке 

private:
    
    template <class T>
    Error process(T&& pos)
    {
        return Error::CorruptedArchive;
    }

    Error process(bool& value)
    {
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

    Error process(uint64_t& pos)
    {
        std::string text;
        std::stringstream ss;
        
        in_ >> text;
        
        if (text[0] == '-')
            return Error::CorruptedArchive;

        ss << text;
        ss >> pos;

        return Error::NoError;

    }

    template <class T, class... Args>
    Error process(T&& pos, Args&&... args)
    {
        if (process(pos) == Error::NoError)
            return process(std::forward<Args>(args)...);
        else
            return Error::CorruptedArchive;
    }

};