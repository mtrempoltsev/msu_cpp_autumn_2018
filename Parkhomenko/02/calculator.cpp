#include <iostream>
#include "stdlib.h"
#include "string.h"

using namespace std;

class Calculator {
    std::string string;
public: 
    int64_t result;
    Calculator(std::string str) {
        string = str;
     	result = 0;
    }

    void delete_gap() {
        int count = 0;
        while (count < string.length()) {
            if (string[count] == ' ') {
                string = string.erase(count, 1);
                continue;
            }
            ++count;
        }
        return;
    }

    int convert_arg(const std::string str, int64_t& arg) {
        arg = 0;
        int ind = 0;
        if (str.length() == 0) {
            std::cout << "error" << std::endl;
            return 1;
        }
        if (str[ind] == '-') {
            ++ind;
            for (int i = ind; i < str.length(); ++i) {
                char number[1];
                if (not isdigit(str[i])) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                number[0] = str[i];
                arg = arg * 10 + atoi(const_cast<char*>(number));
            }
            arg = -arg;
        } else {
            for (int i = ind; i < str.length(); ++i) {
                char number[1];
                if (not isdigit(str[i])) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                number[0] = str[i];
                arg = arg * 10 + atoi(const_cast<char*>(number));
            }
        }
        return 0;
    }

    int define_result_multiply_divide(const std::string string_local, int64_t& result) {
        int n_first_arg = 0;
        for (int i = 0; i < string_local.length(); i++) {
            if (string_local[i] == '*') {
                std::string str1 = string_local;
                std::string str2 = string_local;
                int error1, error2;
                int64_t arg1, arg2;
                error1 = convert_arg(str1.erase(n_first_arg, str1.length() - n_first_arg), arg1);
                error2 = define_result_multiply_divide(str2.erase(0, n_first_arg + 1), arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 * arg2;
                return 0;
            } 
            if (string_local[i] == '/') {
                std::string str1 = string_local;
                std::string str2 = string_local;
                int error1, error2;
                int64_t arg1, arg2;
                error1 = convert_arg(str1.erase(n_first_arg, str1.length() - n_first_arg), arg1);
                error2 = define_result_multiply_divide(str2.erase(0, n_first_arg + 1), arg2);
                if (error1 || error2) {
                    return 1;
                }
                if (arg2 == 0) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                result = arg1 / arg2;
                return 0;
            }    
            ++n_first_arg;
        }
        int error = convert_arg(string_local, result);
        if (error) { 
            return 1;
        }
        return 0;
    }

    int define_result_plus_minus(const std::string string_local, int64_t& result) {
        int n_first_arg = 0;
        for (int i = 0; i < string_local.length(); i++) {
            if (string_local[i] == '+') {
                std::string str1 = string_local;
                std::string str2 = string_local;
                int error1, error2;
                int64_t arg1, arg2;
                error1 = define_result_multiply_divide(str1.erase(n_first_arg, str1.length() - n_first_arg), arg1);
                error2 = define_result_plus_minus(str2.erase(0, n_first_arg + 1), arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 + arg2; 
                return 0;
            } 
            if ((string_local[i] == '-') && (i > 0) && (i + 1 < string_local.length()) && \
                    (isdigit(string_local[i+1])) && (isdigit(string_local[i-1]))) {
                std::string str_minus = "-";
                std::string str1 = string_local;
                std::string str2 = string_local;
                int error1, error2;
                int64_t arg1, arg2;
                error1 = define_result_multiply_divide(str1.erase(n_first_arg, str1.length() - n_first_arg), arg1);
                error2 = define_result_plus_minus(str_minus + str2.erase(0, n_first_arg + 1), arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 + arg2;
                return 0;
            }
            if ((string_local[i] == '-') && (i > 0) && (i + 1 < string_local.length()) && \
                    (string_local[i+1] == '-') && (isdigit(string_local[i-1]))) {
                std::string str1 = string_local;
                std::string str2 = string_local;
                int error1, error2;
                int64_t arg1, arg2;
                error1 = define_result_multiply_divide(str1.erase(n_first_arg, str1.length() - n_first_arg), arg1);
                error2 = define_result_plus_minus(str2.erase(0, n_first_arg + 2), arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 + arg2;
                return 0;
            }
            ++n_first_arg;	
        }    
        int error = define_result_multiply_divide(string_local, result);
        if (error) { 
            return 1;
        }
        return 0;
    }

    int main_calc() {
        int error;
        error = define_result_plus_minus(string, result);
        return error;
    }
};


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    } 
    std::string string = argv[1];
    int error;
    Calculator calc = Calculator(string); 	
    calc.delete_gap();
    error = calc.main_calc();
    if (error) {
        return 1;
    }
    std::cout << calc.result << std::endl;
    return 0;
}
