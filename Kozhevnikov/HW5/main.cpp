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
    Error operator()(ArgsT &&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream &out_;

    Error out_obj(bool &obj) {
        if (obj) {
            out_ << "true";
        } else {
            out_ << "false";
        }
        out_ << Separator;
        return Error::NoError;
    }

    Error out_obj(uint64_t &obj) {
        out_ << obj << Separator;

        return Error::NoError;
    }

    template <class T>
    Error out_obj(T &obj) {
        return Error::CorruptedArchive;
    }

    template<class T>
    Error process(T &val) {
        if (out_obj(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    template<class T, class... Args>
    Error process(T &val, Args &&... args) {
        if (out_obj(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }

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
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::istream &in_;

    Error in_obj(bool &obj) {
        std::string a;
        in_ >> a;
        if (a == "true") {
            obj = true;
        } else if (a == "false") {
            obj = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error in_obj(uint64_t &obj) {
        std::string a;
        in_ >> a;
        if (a.empty()) {
            return Error::CorruptedArchive;
        } else if (a[0] == '-') {
            return Error::CorruptedArchive;
        } else {
            try {
                obj = stoul(a);
            } catch (std::logic_error&) {
                return Error::CorruptedArchive;
            }
        }
        return Error::NoError;
    }

    template <class T>
    Error in_obj(T &val) {
        return Error::CorruptedArchive;
    }

    template <class T>
    Error process (T &val) {
        return in_obj(val);
    }

    template <class T, class... Args>
    Error process (T &val, Args &&... args) {
        if (in_obj(val) == Error::CorruptedArchive) {
            return Error::CorruptedArchive;
        }
        return process(std::forward<Args>(args)...);
    }
};
