#include <iostream>
#include <sstream>
#include <cstring>
#include <typeinfo>

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
        if (typeid(val) == typeid(bool)) {
            if (val) {
                out_ << "true" << Separator;
            } else {
                out_ << "false" << Separator;
            }
        } else if (typeid(val) == typeid(uint64_t)) {
            out_ << val << Separator;
        } else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    template<class T, class... Args>
    Error process(T &&val, Args &&... args) {
        if (typeid(val) == typeid(bool)) {
            if (val) {
                out_ << "true" << Separator;
            } else {
                out_ << "false" << Separator;
            }
        } else if (typeid(val) == typeid(uint64_t))
            out_ << val << Separator;
        else return Error::CorruptedArchive;

        return process(std::forward<Args>(args)...);
    }
};

class Deserializer {
public:
    explicit Deserializer(std::istream &in)
            : in_(in) {}

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

    template<class ...Args>
    Error process(bool &val, Args &... args) {
        std::string a;
        in_ >> a;
        if (a == "true") {
            val = true;
        } else if (a == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }

        return process(args...);
    }

    template<class ...Args>
    Error process(uint64_t &val, Args &... args) {
        std::string a;
        uint64_t buf;
        in_ >> a;
        if (a.empty()) {
            return Error::CorruptedArchive;
        } else if (a[0] == '-') {
            return Error::CorruptedArchive;
        } else {
            try {
                buf = stoul(a);
            } catch (...) {
                return Error::CorruptedArchive;
            }
            val = buf;
        }

        return process(args...);
    }

    Error process(bool &val) {
        std::string a;
        in_ >> a;
        if (a == "true") {
            val = true;
        } else if (a == "false") {
            val = false;
        } else {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error process(uint64_t &val) {
        std::string a;
        uint64_t buf;
        in_ >> a;
        if (a.empty()) {
            return Error::CorruptedArchive;
        } else if (a[0] == '-') {
            return Error::CorruptedArchive;
        } else {
            try {
                buf = stoul(a);
            } catch (...) {
                return Error::CorruptedArchive;
            }
            val = buf;
        }

        return Error::NoError;
    }
};
