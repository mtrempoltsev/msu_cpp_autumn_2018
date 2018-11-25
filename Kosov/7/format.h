#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>


template <class T>
std::string to_str(T&& x){
    std::ostringstream buf;
    buf << x;
    return buf.str();
}

template <class... Types>
std::string format(std::string x, Types&&... t){
    std::vector<std::string> args = {to_str(std::forward<Types>(t))...};
    std::string result;
    std::istringstream in(x);
    char buf;
    while(in.read(&buf, 1)){
        if(buf == '{'){
            int k;
            in >> k;
            if(k+1 > args.size())
                throw std::runtime_error("");
            result += args[k];
            in >> buf;
            if(buf != '}')
                throw std::runtime_error("");
        }else{
            if(buf == '}')
                throw std::runtime_error("");
            result += buf;
        }
    }
    return result;
}
