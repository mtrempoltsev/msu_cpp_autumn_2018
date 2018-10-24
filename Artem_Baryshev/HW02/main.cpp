//
//  main.cpp
//  HW2
//
//  Created by Артем Барышев on 23/10/2018.
//  Copyright © 2018 Артем Барышев. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <string>

enum
{
    BASE = 10
};


class calculator {
    std::string s;
    bool correct_syntax;
    int64_t ans;
    
    bool check_correctness() const
    {
        bool is_first_number = true;
        int count_last_sigh = 0;
        for (int i = 0; i < s.size(); i++) {
            
            if ((s[i] < '0' || s[i] > '9') && !isspace(s[i])
                && s[i] != '-' && s[i] != '+' && s[i] != '*' && s[i] != '/') {
                return false;          // в строке некорректные символы
            }
            
            if (s[i] == '-') {
                if (count_last_sigh >= 2 || (is_first_number && count_last_sigh >= 1)) {
                    return false;
                }
                count_last_sigh++;
            }
            
            if (s[i] == '+' || s[i] == '*' || s[i] == '/') {
                if (is_first_number || count_last_sigh >= 1) {
                    return false;
                }
                count_last_sigh++;
            }
            
            if (s[i] >= '0' && s[i] <= '9') {
                if (count_last_sigh == 0 && !is_first_number) {   //два числа подряд без арифмитической операции
                    return false;
                }
                while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
                    i++;
                }
                i--;
                is_first_number = false;
                count_last_sigh = 0;
            }
        }
        if (count_last_sigh >= 1 || is_first_number) {
            return false;
        }
        return true;
    }
    
    void removing_spaces() {
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
    }
    
    size_t division_and_multiplication(int64_t & first_number, char & operation, size_t pos) {
        while (operation == '/' || operation == '*') {
            size_t next_pos;
            int64_t now = stoll(std::string(s.begin() + pos, s.end()), &next_pos, BASE);
            pos += next_pos;
            if (operation == '/') {
                if (now == 0) {
                    correct_syntax = false;
                    return pos;
                }
                first_number /= now;
            } else {
                first_number *= now;
            }
            operation = s[pos++];
        }
        return pos;
    }
    
    int64_t summation_and_difference(char last_operation, size_t pos = 0) {
        
        if (pos >= s.size()) {
            return 0;
        }
        size_t next_pos;
        int64_t sum = stoll(std::string(s.begin() + pos, s.end()), &next_pos, BASE);
        pos += next_pos;
        if (last_operation == '-') {
            sum = -sum;
        }
        
        if (pos >= s.size()) {
            return sum;
        }
        char operation = s[pos];
        pos++;
        
        if (operation == '*' || operation == '/') {
            pos = division_and_multiplication(sum, operation, pos);
        }
        
        return sum + summation_and_difference(operation, pos);
    }
    
public:
    
    calculator(char *string_now): s(std::string(string_now))
    {
        correct_syntax = check_correctness();
        if (correct_syntax) {
            removing_spaces();
            ans = summation_and_difference('+');
        }
    }
    
    int64_t get_ans() {
        return ans;
    }
    
    bool get_correct_syntax() {
        return correct_syntax;
    }
    
};

int main(int argc, char * argv[]) {
    if (argc == 2) {
        calculator my_calculator(argv[1]);
        if (!my_calculator.get_correct_syntax()) {
            std::cout << "error\n";
            return 1;
        }
        std::cout << my_calculator.get_ans() << std::endl;
        
    } else {
        std::cout << "error\n";
        return 1;
    }
    return 0;
}
