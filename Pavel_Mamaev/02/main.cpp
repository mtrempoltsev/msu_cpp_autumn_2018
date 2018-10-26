//
//  main.cpp
//  calc-class
//
//  Created by Мамаев Павел on 23/10/2018.
//  Copyright © 2018 Мамаев Павел. All rights reserved.
//

#include <iostream>
#include <string>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
using namespace std;

class calc_expression {
public:
    bool fail;
    calc_expression(string &s, size_t npos = 0) : exp(s), fail(false), pos(npos), res(0) {
    }
    void del_spaces() {
        exp.erase(remove(exp.begin(), exp.end(), ' '), exp.end());
    }
    int64_t get_res() {
        return res;
    }
    bool calculate_res() {
        res = calculate();
        return fail;
    }
    ~calc_expression() {
    }
private:
    string exp;
    size_t pos;
    int64_t res;
    int64_t take_number() {
        size_t last_pos = pos;
        while (pos < exp.size() && exp[pos] == '-') {
            ++pos;
        }
        if (exp[pos] < '0' || exp[pos] > '9') {
            fail = true;
            return 1;
        }
        int64_t tmp_sign = 1;
        if (((pos - last_pos) & 1) == 1) {
            tmp_sign = -1;
        }
        int64_t elem = get_num(pos);
        return elem * tmp_sign;
    }
    int64_t get_num(size_t &pos) {
        int64_t res = 0;
        char *st = &exp[pos];
        char *end;
        res = (int64_t) strtoll(st, &end, 10);
        ptrdiff_t diff = end - st;
        pos = pos + (size_t) diff;
        return res;
    }
    int64_t calculate(bool fast = false) {
        int64_t first = take_number();
        if (fail) {
            return 1;
        }
        while (exp[pos]) {
            char oper = exp[pos];
            if (fast || oper == '\0') {
                return first;
            }
            ++pos;
            if (oper == '-' || oper == '+') {
                if (oper == '-') {
                    pos--;
                }
                calc_expression temp(exp, pos);
                fail = temp.calculate_res();
                int64_t second = temp.get_res();
                return first + second;
            } else if (oper == '*') {
                int64_t second = calculate(true);
                if (fail) {
                    return 1;
                }
                first *= second;
            } else if (oper == '/') {
                int64_t second = calculate(true);
                if (fail || second == 0) {
                    fail = true;
                    return 1;
                }
                first /= second;
            }
        }
        return first;
    }
};
int main(int argc, const char * argv[]) {
    if (argc != 2){
        cout << "error" << endl;
        return 1;
    }
    string a = argv[1];
    calc_expression A(a);
    A.del_spaces();
    if (A.calculate_res()){
        cout << "error" << endl;
        return 1;
    } else {
        cout << A.get_res() << endl;
    }
    return 0;
}
