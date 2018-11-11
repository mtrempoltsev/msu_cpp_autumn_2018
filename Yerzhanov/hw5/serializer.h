#include <iostream>
#include <string>


enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
private:
    std::ostream& out_;
    static constexpr char sep = ' ';
public:
    Serializer(std::ostream& out)
            : out_(out) {}

    template <class T>
    Error save(T& item) {
        return item.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args... args) {
        return serialize(args...);
    }
private:
    template <class T, class... Args>
    Error serialize(T& item, Args&... args) {
        if (serialize(item) == Error::NoError) {
            return serialize(args...);
        } else {
            return Error::CorruptedArchive;
        }
    }

    Error serialize(bool item) {
        out_ << (item ? "true" : "false") << sep;
        return Error::NoError;
    }

    Error serialize(uint64_t item) {
        out_ << item << sep;
        return Error::NoError;
    }

    template <class T>
    Error serialize(T& item) {
        return Error::CorruptedArchive;
    }
};

class Deserializer {
private:
    std::istream& in_;
public:
    Deserializer(std::istream& in)
            :   in_(in) {}

    template<class T>
    Error load(T& obj) {
        return obj.serialize(*this);
    }

    template<class... Args>
    Error operator()(Args&... args) {
        return deserialize(args...);
    }
private:
    template<class T, class... Args>
    Error deserialize(T& item, Args&... args) {
        if (deserialize(item) == Error::NoError) {
            return deserialize(args...);
        } else {
            return Error::CorruptedArchive;
        }
    }

    Error deserialize(bool& item) {
        std::string tmp;
        in_ >> tmp;
        if (tmp == "true" || tmp == "false") {
            item = tmp == "true";
            return Error::NoError;
        } else {
            return Error::CorruptedArchive;
        }
    }

    Error deserialize(uint64_t& item) {
        std::string tmp;
        in_ >> tmp;
        item = 0;
        for (size_t i = 0; i < tmp.size(); ++i) {
            if (std::isdigit(tmp[i])) {
                item = item * 10 + tmp[i] - '0';
            } else {
                return Error::CorruptedArchive;
            }
        }
        return Error::NoError;
    }
};
