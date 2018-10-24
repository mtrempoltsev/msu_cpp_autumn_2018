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

    int convert_arg() {
        result = 0;
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
                result = result * 10 + atoi(const_cast<char*>(number));
            }
            result = -result;
        } else {
            for (int i = ind; i < string.length(); ++i) {
                char number[1];
                if (not isdigit(string[i])) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                number[0] = string[i];
                result = result * 10 + atoi(const_cast<char*>(number));
            }
        }
        return 0;
    }

    int define_result_multiply_divide() {
        int n_first_arg = 0;
        for (int i = 0; i < string.length(); i++) {
            if (string[i] == '*') {
                std::string str = string;
                Calculator arg1 = Calculator(string.erase(n_first_arg, string.length() - n_first_arg));
                Calculator arg2 = Calculator(str.erase(0, n_first_arg + 1));
                int error1, error2;
                error1 = arg1.convert_arg();
                error2 = arg2.define_result_multiply_divide();
                if (error1 || error2) {
                    return 1;
                }
                result = arg1.result * arg2.result;
                return 0;
            } 
            if (string[i] == '/') {
                std::string str = string;
                Calculator arg1 = Calculator(string.erase(n_first_arg, string.length() - n_first_arg));
                Calculator arg2 = Calculator(str.erase(0, n_first_arg + 1));
                int error1, error2;
                error1 = arg1.convert_arg();
                error2 = arg2.define_result_multiply_divide();
                if (error1 || error2) {
                    return 1;
                }
                if (arg2.result == 0) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                result = arg1.result / arg2.result;
                return 0;
            }    
            ++n_first_arg;
        }
        int error = this->convert_arg();
        if (error) { 
            return 1;
        }
        return 0;
    }

    int define_result_plus_minus() {
        int n_first_arg = 0;
        for (int i = 0; i < string.length(); i++) {
            if (string[i] == '+') {
                std::string str = string;
                Calculator arg1 = Calculator(string.erase(n_first_arg, string.length() - n_first_arg));
                Calculator arg2 = Calculator(str.erase(0, n_first_arg + 1));
                int error1, error2;
                error1 = arg1.define_result_multiply_divide();
                error2 = arg2.define_result_plus_minus();
                if (error1 || error2) {
                    return 1;
                }
                result = arg1.result + arg2.result; 
                return 0;
            } 
            if ((string[i] == '-') && (i > 0) && (i + 1 < string.length()) && \
                    (isdigit(string[i+1])) && (isdigit(string[i-1]))) {
                std::string str_minus = "-";
                std::string str = string;
                Calculator arg1 = Calculator(string.erase(n_first_arg, string.length() - n_first_arg));
                Calculator arg2 = Calculator(str_minus + str.erase(0, n_first_arg + 1));
                int error1, error2;
                error1 = arg1.define_result_multiply_divide();
                error2 = arg2.define_result_plus_minus();
                if (error1 || error2) {
                    return 1;
                }
                result = arg1.result + arg2.result;
                return 0;
            }
            if ((string[i] == '-') && (i > 0) && (i + 1 < string.length()) && \
                    (string[i+1] == '-') && (isdigit(string[i-1]))) {
                int64_t args[2];
                std::string str = string;
                Calculator arg1 = Calculator(string.erase(n_first_arg, string.length() - n_first_arg));
                Calculator arg2 = Calculator(str.erase(0, n_first_arg + 2));
                int error1, error2;
                error1 = arg1.define_result_multiply_divide();
                error2 = arg2.define_result_plus_minus();
                if (error1 || error2) {
                    return 1;
                }
                result = arg1.result + arg2.result;
                return 0;
            }
            ++n_first_arg;	
        }    
        int error = this->define_result_multiply_divide();
        if (error) { 
            return 1;
        }
        return 0;
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
    error = calc.define_result_plus_minus();
    if (error) {
        return 1;
    }
    std::cout << calc.result << std::endl;
    return 0;
}
