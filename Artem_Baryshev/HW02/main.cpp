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

enum
{
    BASE = 10
};


class calculator {
    char *s;
    const size_t size_s;
    bool correct_syntax;
    int64_t ans;
    
    bool check_correctness() const
    {
        bool is_first_number = 0;
        int count_last_sigh = 0;
        for (int i = 0; i < size_s; i++) {
            
            if ((s[i] < '0' || s[i] > '9') && !isspace(s[i])
                && s[i] != '-' && s[i] != '+' && s[i] != '*' && s[i] != '/') {
                return false;          // в строке некорректные символы
            }
            
            if (s[i] == '-') {
                if (count_last_sigh >= 2 || (!is_first_number && count_last_sigh >= 1)) {
                    return false;
                }
                count_last_sigh++;
            }
            
            if (s[i] == '+' || s[i] == '*' || s[i] == '/') {
                if (!is_first_number || count_last_sigh >= 1) {
                    return false;
                }
                count_last_sigh++;
            }
            
            if (s[i] >= '0' && s[i] <= '9') {
                if (count_last_sigh == 0 && is_first_number) {   //два числа подряд без арифмитической операции
                    return false;
                }
                is_first_number = true;
                count_last_sigh = 0;
            }
            
        }
        if (count_last_sigh >= 1 || !is_first_number) {
            return false;
        }
        return true;
    }
    
    void removing_spaces() {
        char *now = s;
        char *next = s;
        while (*next) {
            while (*next == ' ') {
                next++;
            }
            *now = *next;
            next++;
            now++;
        }
        *now = *next;
    }
    
    void division_and_multiplication(int64_t & first_number, char & operation) {
        while (operation == '/' || operation == '*') {
            int64_t now = strtoll(s, &s, BASE);
            if (operation == '/') {
                if (now == 0) {
                    correct_syntax = false;
                    return;
                }
                first_number /= now;
            } else {
                first_number *= now;
            }
            operation = *s;
            s++;
        }
    }
    
    int64_t summation_and_difference(char last_operation) {
        
        if (!*s) {
            return 0;
        }
        int64_t sum = strtoll(s, &s, BASE);
        if (last_operation == '-') {
            sum = -sum;
        }
        
        char operation = *s;
        if (!*s) {
            return sum;
        }
        s++;
        
        if (operation == '*' || operation == '/') {
            division_and_multiplication(sum, operation);
        }
        
        return sum + summation_and_difference(operation);
    }
    
public:
    
    calculator(char *string_now): s(string_now), size_s(strlen(s))
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
