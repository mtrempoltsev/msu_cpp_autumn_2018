#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stdexcept>

template<class T>
std::string args_to_str(T&& arg) {
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

template<class... ArgsT>
std::string format(const char *sz, ArgsT&&... args) {
    std::vector< std::string > arr_args({args_to_str(std::forward< ArgsT >(args))... });
    
    std::stringstream for_ret;

    std::size_t len = std::strlen(sz);
    for (std::size_t i = 0; i < len; ++i) {
        if (sz[i] == '}') {
            throw std::runtime_error("Wrong format");
        }

        if (sz[i] == '{') {
            ++i;

            int r;
            std::size_t pos;

            try {
                r = std::stoull(sz + i, &pos);
            } catch (const std::invalid_argument &e) {
                throw std::runtime_error("Wrong format in {}");
            } catch (const std::out_of_range &e) {
                throw std::invalid_argument("Wrong format in {}. Too large num");
            }
            if (sz[i + pos] != '}') {
                throw std::runtime_error("Wrong format in {}. Not }");
            }

            if (0 <= r && std::size_t(r) < arr_args.size()) {
                for_ret << arr_args[r];
            } else {
                throw std::runtime_error("Wrong format in {}. Num not in limit");
            }

            i += pos;

        } else {
            for_ret << sz[i];
        }
    }

    return for_ret.str();
}
