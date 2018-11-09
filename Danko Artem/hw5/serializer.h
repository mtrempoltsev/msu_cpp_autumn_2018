#include <iostream>
#include <sstream>

enum class Error
{
    NoError,
    CorruptedArchive
};


class Serializer
{
    std::ostream& out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }

private:
    template <class T, class... ArgsT>
    Error process(T&& val, ArgsT&&... args) {
        if(process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(std::forward<ArgsT>(args)...);
    }
    Error process(bool val){
        out_ << ((val == true)?("true"):("false")) << Separator;
        return Error::NoError;
    }
    Error process(uint64_t val){
        out_ << val << Separator;
        return Error::NoError;
    }
    template <class T>
    Error process(T&& val){
        return Error::CorruptedArchive;
    }
};

class Deserializer
{
    std::istream& in_;
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }

private:
    template <class T, class... ArgsT>
    Error process(T& val, ArgsT&... args) {
        if(process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(args...);
    }
    Error process(bool& val){
        std::string str;
        in_>>str;
        if(str == "true"){
            val = true;
        } else if (str == "false"){
            val = false;
        } else return Error::CorruptedArchive;
        return Error::NoError;
    }
    Error process(uint64_t& val){
        std::string temp;
        in_>>temp;
        if(temp.c_str()[0] == '-')
            return Error::CorruptedArchive;
        std::stringstream s(temp);
        s>>val;
        return Error::NoError;
    }
    template <class T>
    Error process(T& val){
        return Error::CorruptedArchive;
    }
};

