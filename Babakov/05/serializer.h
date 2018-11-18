#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{    
    static constexpr char Separator = ' ';
    std::ostream& out_;

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
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&& ... args)
    {
        if (process(std::forward<T>(value)) == Error::NoError) 
            return process(std::forward<ArgsT>(args)...);
        else
            return Error::CorruptedArchive;
    }

    Error process(bool value)
    {
        if (out_ << (value == true ? 
                "true" : "false") << Separator)
            return Error::NoError;
        else
            return Error::CorruptedArchive;
    }    

    Error process(uint64_t value)
    {
        if (out_ << value << Separator)
            return Error::NoError;
        else
            return Error::CorruptedArchive;
    }
};

class Deserializer
{
    std::istream& in_;

public:
    explicit Deserializer(std::istream& in): in_(in)
    {
    };

    template<class... ArgsT>
    Error operator()(ArgsT&& ... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
    
    template<class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

private:
    template<class T, class... ArgsT>
    Error process(T&& value, ArgsT&& ... args)
    {
        if (process(std::forward<T>(value)) == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        } else {
            return Error::CorruptedArchive;
        }
    }

    Error process(uint64_t& value)
    {
        std::string buf;
        in_ >> buf;

        if (buf.empty() || !isdigit(buf[0]) || buf[0] == '-') {
            return Error::CorruptedArchive;
        } else {
            value = std::stoull(buf.c_str());
            return Error::NoError;
        }
    }

    Error process(bool& value)
    {
        std::string buf;
        in_ >> buf;

        if (buf == "true")
            value = true;
        else if (buf == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }
};