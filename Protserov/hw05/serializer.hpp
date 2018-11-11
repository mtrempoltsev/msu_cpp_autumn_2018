#ifndef MYSERIALIZER_H
#define MYSERIALIZER_H

#include <iostream>
#include <string>

inline bool isInteger(const std::string& s)
{
    if (s.empty() || s[0] == '-' || ((!isdigit(s[0])) && (s[0] != '+'))) {
        return false;
    }

    char* p;
    strtoull(s.c_str(), &p, 10);

    return (*p == 0);
}

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer
{
        static constexpr char Separator = ' ';
        std::ostream& out_;

        template<class T, class... ArgsT>
        Error process(T&& val, ArgsT&& ... args)
        {
            if (process(val) == Error::NoError) {
                return process(std::forward<ArgsT>(args)...);
            } else {
                return Error::CorruptedArchive;
            }
        }

        Error process(uint64_t val)
        {
            if (!(out_ << val << Separator)) {
                return Error::CorruptedArchive;
            } else {
                return Error::NoError;
            }
        }

        Error process(bool val)
        {
            const char* outval;
            if (val == true) {
                outval = "true";
            } else {
                outval = "false";
            }
            if (!(out_ << outval << Separator)) {
                return Error::CorruptedArchive;
            } else {
                return Error::NoError;
            }
        }

        template<class... ArgsT>
        Error process()
        {
            return Error::NoError;
        }

    public:
        explicit Serializer(std::ostream& out): out_(out)
        {
        };

        template<class T>
        Error save(T& object)
        {
            return object.serialize(*this);
        }

        template<class... ArgsT>
        Error operator()(ArgsT... args)
        {
            return process(args...);
        }
};

class Deserializer
{
        std::istream& in_;

        template<class T, class... ArgsT>
        Error process(T&& val, ArgsT&& ... args)
        {
            if (process(std::forward<T>(val)) == Error::NoError) {
                return process(std::forward<ArgsT>(args)...);
            } else {
                return Error::CorruptedArchive;
            }
        }

        Error process(uint64_t& val)
        {
            std::string text;
            in_ >> text;
            if (isInteger(text)) {
                val = std::stoull(text.c_str());
                return Error::NoError;
            } else {
                return Error::CorruptedArchive;
            }
        }

        Error process(bool& val)
        {
            std::string text;
            in_ >> text;
            if (text == "true") {
                val = true;
                return Error::NoError;
            } else if (text == "false") {
                val = false;
                return Error::NoError;
            } else {
                return Error::CorruptedArchive;
            }
        }

        template<class... ArgsT>
        Error process()
        {
            return Error::NoError;
        }

    public:
        explicit Deserializer(std::istream& in): in_(in)
        {
        };

        template<class T>
        Error load(T& object)
        {
            return object.serialize(*this);
        }

        template<class... ArgsT>
        Error operator()(ArgsT&& ... args)
        {
            return process(std::forward<ArgsT>(args)...);
        }
};
#endif
