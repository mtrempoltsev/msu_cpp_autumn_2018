#include <iostream>
#include "stdlib.h"
#include "string.h"

using namespace std;

int64_t convert_arg(string string) {
    int64_t arg = 0;
    int ind = 0;
    if (string.length() == 0) {
        std::cout << "error" << std::endl;
        exit (1);
    }
    if (string[ind] == '-') {
        ++ind;
        for (int i = ind; i < string.length(); ++i) {
            char number[1];
            if (not isdigit(string[i])) {
                std::cout << "error" << std::endl;
                exit (1);
            }
            number[0] = string[i];
            arg = arg * 10 + atoi(const_cast<char*>(number));
        }
        arg = -arg;
    } else {
        for (int i = ind; i < string.length(); ++i) {
            char number[1];
            if (not isdigit(string[i])) {
                std::cout << "error" << std::endl;
                exit (1);
            }
            number[0] = string[i];
            arg = arg * 10 + atoi(const_cast<char*>(number));
        }
    }
    return arg;
}

int64_t define_result_multiply_divide(string string) {
    int n_first_arg = 0;
    for (int i = 0; i < string.length(); i++) {
        if (string[i] == '*') {
            int64_t args[2];
            std::string str = string;
            args[0] = convert_arg(string.erase(n_first_arg, string.length() - n_first_arg));
            args[1] = define_result_multiply_divide(str.erase(0, n_first_arg + 1));
            return (args[0] * args[1]);
        } 
        if (string[i] == '/') {
            int64_t args[2];
            std::string str = string;
            args[0] = convert_arg(string.erase(n_first_arg, string.length() - n_first_arg));
            args[1] = define_result_multiply_divide(str.erase(0, n_first_arg + 1));
            if (args[1] == 0) {
                std::cout << "error" << std::endl;
                exit (1);
            }
            return (args[0] / args[1]);
        }
        ++n_first_arg;
    }
    return convert_arg(string);
}

int64_t define_result_plus_minus(string string) {
    int n_first_arg = 0;
    for (int i = 0; i < string.length(); i++) {
        if (string[i] == '+') {
            int64_t args[2];
            std::string str = string;
            args[0] = define_result_multiply_divide(string.erase(n_first_arg, string.length() - n_first_arg));
            args[1] = define_result_plus_minus(str.erase(0, n_first_arg + 1));
            return (args[0] + args[1]);
        } 
        if ((string[i] == '-') && (i > 0) && (i + 1 < string.length()) && \
                (isdigit(string[i+1])) && (isdigit(string[i-1]))) {
            std::string str_minus = "-";
            int64_t args[2];
            std::string str = string;
            args[0] = define_result_multiply_divide(string.erase(n_first_arg, string.length() - n_first_arg));
            args[1] = define_result_plus_minus(str_minus + str.erase(0, n_first_arg + 1));
            return (args[0] + args[1]);
        }
        if ((string[i] == '-') && (i > 0) && (i + 1 < string.length()) && \
                (string[i+1] == '-') && (isdigit(string[i-1]))) {
            int64_t args[2];
            std::string str = string;
            args[0] = define_result_multiply_divide(string.erase(n_first_arg, string.length() - n_first_arg));
            args[1] = define_result_plus_minus(str.erase(0, n_first_arg + 2));
            return (args[0] + args[1]);
        }
        ++n_first_arg;	
    }
    return define_result_multiply_divide(string);
}

string delete_gap(string string) {
    int count = 0;
    while (count < string.length()) {
        if (string[count] == ' ') {
            string = string.erase(count, 1);
            continue;
        }
        ++count;
    }
    return string;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "error" << std::endl;
        exit (1);
    } 
    std::string string = argv[1];
    std::cout << define_result_plus_minus(delete_gap(string)) << std::endl;
    return 0;
}
