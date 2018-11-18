#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

class getter {
public:
    std::vector<std::string> a;
    template <class... ArgsT>
    getter(ArgsT&&... args) {
        get(std::forward<ArgsT>(args)...);
    }
    std::vector<std::string> get_buf() {
        return a;
    }
private:
    template <class T, class... ArgsT>
    void get(T&& a1, ArgsT&&... args) {
        get(a1);
        get(std::forward<ArgsT>(args)...);
    }
    template <class T>
    void get(T&& a1) {
        std::stringstream ss;
        ss << a1;
        a.push_back(ss.str());
    }
    void get(){}
};


template <class... ArgsT>
std::string format(const char* a, ArgsT... args) {
    std::vector<std::string> sargs = getter(std::forward<ArgsT>(args)...).get_buf();
    std::stringstream ss;
    for (auto i = 0; a[i] != '\0'; i++) {
        if (a[i] == '}')
            throw std::runtime_error("where is my {?");
        if (a[i] == '{') {
            int ans = 0;
            while (a[++i] != '}') {
                if (a[i] <= '9' && a[i] >= '0') {
                    ans = ans * 10 + a[i] - '0';
                } else {
                    throw std::runtime_error("where is my }?");
                }
            }
            if (ans >=sargs.size())
                throw std::runtime_error("what is the number?");
            ss << sargs[ans];
        }
        else {
            ss << a[i];
        }
    }
    return ss.str();
}
