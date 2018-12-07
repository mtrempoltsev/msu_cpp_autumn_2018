#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

template<class T>
string to_string(T&& arg) {
    stringstream sstr;
    sstr << arg;
    return sstr.str();
}

template <class... Args>
string format(const string& s, Args&&... args) {
    vector<string> argv{to_string(forward<Args>(args))...};

    string res;

    int braces_balance = 0;
    string current = "";
    for (char c : s) {
        if (c == '{') {
            if (braces_balance > 0) {
                throw runtime_error("excess {}");
            }
            braces_balance += 1;
        } else if (c == '}') {
            if (braces_balance == 0) {
                throw runtime_error("excess {}");
            }
            braces_balance -= 1;
            if (current.size() == 0) {
                throw runtime_error("empty {}");
            }
            int idx = stoi(current);
            if (idx < 0 || idx >= argv.size()) {
                throw runtime_error("index error");
            } else {
                res.append(argv[idx]);
            }
            current = "";
        } else if (braces_balance == 0) {
            res.push_back(c);
        } else {
            if ((c < '0') || (c > '9')) {
                throw runtime_error("not number in {}");
            } else {
                current += c;
            }
        }
    }
    if (braces_balance > 0) {
        throw runtime_error("excess {}");
    }
    return res;
}
