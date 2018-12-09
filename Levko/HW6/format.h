#include <string>
#include <vector>
#include <stdexcept>


template <class T>
std::string to_str(T&& temp) {
    std::ostringstream out;
    out << temp;
    return out.str();
}

template <class... Types>
std::string format(std::string str, Types&&... t) {
    std::string result;
    std::istringstream in(str);
    std::vector<std::string> args = {to_str(std::forward<Types>(t))...};
    char buf;
    in >> std::noskipws;
    while(in >> buf) {
        if(buf == '{') {
            int k;
            in >> k;
            if(k+1 > args.size())
                throw std::runtime_error("");
            result += args[k];
            in >> buf;
            if(buf != '}')
                throw std::runtime_error("");
        } else {
            if(buf == '}')
                throw std::runtime_error("");
            result += buf;
        }
    }
    return result;
}
 
