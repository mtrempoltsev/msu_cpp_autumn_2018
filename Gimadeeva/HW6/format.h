#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

string format(const string& s) {
    return s;
}

template<class T>
string to_string(T arg) {
    stringstream sstr;
    sstr << arg;
    return sstr.str();
}

template <typename T, typename... Args>
string format(const string& s, T&& t, Args&&... args) {
    ostringstream ostr;
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
            if (idx < 0 || idx > (int)sizeof...(args)) {
                throw runtime_error("error");
            } else if (idx == 0) {
                ostr << t;
            } else {
                ostr << "{" << idx - 1 << "}";
            }
            current = "";
        } else if (braces_balance == 0) {
            ostr << c;
        } else {
            if ((c < '0') || (c > '9')) {
                throw runtime_error("not number in {}");
            } else {
                current += c;
            }
        }
    }
    ostr.flush();
    if (braces_balance > 0) {
        throw runtime_error("excess {}");
    }
    return format(ostr.str(), forward<Args>(args)...);
}
