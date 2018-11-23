#include <iostream>
#include "stdlib.h"
#include "string.h"

using namespace std;

class Calculator {
    std::string string;
public: 
    int64_t result;
    Calculator(const std::string str) : string(str), result(0) {}

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

    int convert_arg(int64_t& arg) {
        arg = 0;
        int ind = 0;
        if (string.length() == 0) {
            std::cout << "error" << std::endl;
            return 1;
        }
        if (string[ind] == '-') {
            ++ind;
            for (int i = ind; i < string.length(); ++i) {
                char number[1];
                if (not isdigit(string[i])) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                number[0] = string[i];
                arg = arg * 10 + atoi(number);
            }
            arg = -arg;
        } else {
            for (int i = ind; i < string.length(); ++i) {
                char number[1];
                if (not isdigit(string[i])) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                number[0] = string[i];
                arg = arg * 10 + atoi(number);
            }
        }
        return 0;
    }

    int define_result_multiply_divide(int64_t& result) {
        int n_first_arg = 0;
        for (int i = 0; i < string.length(); i++) {
            if (string[i] == '*') {
                std::string str1 = string;
                std::string str2 = string;
                int error1, error2;
                int64_t arg1, arg2;
                string = str1.erase(n_first_arg, str1.length() - n_first_arg);
                error1 = convert_arg(arg1);
                string = str2.erase(0, n_first_arg + 1);
                error2 = define_result_multiply_divide(arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 * arg2;
                return 0;
            } 
            if (string[i] == '/') {
                std::string str1 = string;
                std::string str2 = string;
                int error1, error2;
                int64_t arg1, arg2;
                string = str1.erase(n_first_arg, str1.length() - n_first_arg);
                error1 = convert_arg(arg1);
                string = str2.erase(0, n_first_arg + 1);
                error2 = define_result_multiply_divide(arg2);
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
        int error = convert_arg(result);
        if (error) { 
            return 1;
        }
        return 0;
    }

    int define_result_plus_minus(int64_t& result) {
        int n_first_arg = 0;
        for (int i = 0; i < string.length(); i++) {
            if (string[i] == '+') {
                std::string str1 = string;
                std::string str2 = string;
                int error1, error2;
                int64_t arg1, arg2;
                string = str1.erase(n_first_arg, str1.length() - n_first_arg);
                error1 = define_result_multiply_divide(arg1);
                string = str2.erase(0, n_first_arg + 1);
                error2 = define_result_plus_minus(arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 + arg2; 
                return 0;
            } 
            if ((string[i] == '-') && (i > 0) && (i + 1 < string.length()) && \
                    (isdigit(string[i+1])) && (isdigit(string[i-1]))) {
                std::string str_minus = "-";
                std::string str1 = string;
                std::string str2 = string;
                int error1, error2;
                int64_t arg1, arg2;
                string = str1.erase(n_first_arg, str1.length() - n_first_arg);
                error1 = define_result_multiply_divide(arg1);
                string = str_minus + str2.erase(0, n_first_arg + 1);
                error2 = define_result_plus_minus(arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 + arg2;
                return 0;
            }
            if ((string[i] == '-') && (i > 0) && (i + 1 < string.length()) && \
                    (string[i+1] == '-') && (isdigit(string[i-1]))) {
                std::string str1 = string;
                std::string str2 = string;
                int error1, error2;
                int64_t arg1, arg2;
                string = str1.erase(n_first_arg, str1.length() - n_first_arg);
                error1 = define_result_multiply_divide(arg1);
                string = str2.erase(0, n_first_arg + 2);
                error2 = define_result_plus_minus(arg2);
                if (error1 || error2) {
                    return 1;
                }
                result = arg1 + arg2;
                return 0;
            }
            ++n_first_arg;	
        }    
        int error = define_result_multiply_divide(result);
        if (error) { 
            return 1;
        }
        return 0;
    }

    int main_calc() {
        int error;
        delete_gap();
        error = define_result_plus_minus(result);
        return error;
    }
    
    void print_result() {
        std::cout << result << std::endl;
    }
};


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    } 
    std::string string = argv[1];
    int error;
    Calculator calc(string); 	
    error = calc.main_calc();
    if (error) {
        return 1;
    }
    calc.print_result();
    return 0;
}
