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
    calc_expression(string s) {
        exp = s;
        fail = false;
        pos = 0;
    }
    void del_spaces() {
        exp.erase(remove(exp.begin(), exp.end(), ' '), exp.end());
    }
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
    int64_t get_num(size_t &pos){
        int64_t res = 0;
        while (exp[pos] >= '0' && exp[pos] <= '9') {
            res = res * 10 + exp[pos] - '0';
            ++pos;
        }
        return res;
    }
    int64_t calc(bool fast = false) {
        int64_t first;
        first = take_number();
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
                int64_t second;
                calc_expression temp(exp.substr(pos));
                second = temp.calc();
                fail = temp.fail;
                return first + second;
            } else if (oper == '*') {
                int64_t second = calc(true);
                if (fail) {
                    return 1;
                }
                first *= second;
            } else if (oper == '/') {
                int64_t second = calc(true);
                if (fail || second == 0) {
                    fail = true;
                    return 1;
                }
                first /= second;
            }
        }
        return first;
    }
    bool check() {
        return fail;
    }
    ~calc_expression() {}
private:
    string exp;
    size_t pos;
};
int main(int argc, const char * argv[]) {
    if (argc != 2){
        cout << "error" << endl;
        return 1;
    }
    string a = argv[1];
    calc_expression A(a);
    A.del_spaces();
    int64_t ans = A.calc();
    if (A.check()){
        cout << "error" << endl;
        return 1;
    } else {
        cout << ans << endl;
    }
    return 0;
}
