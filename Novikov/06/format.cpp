#include <vector>
#include <string>


#include <stdexcept>
using namespace std;



template <class T>
string toString(T&& content) {
    ostringstream oo;
    oo << content;
    return oo.str();
}
template <class... Type>
string format(string s, Type&&... type) {
    char c;
    vector<string> arguments = {toString(forward<Type>(type))...};
    istringstream ii(s);
    string formatted;

    ii >> noskipws;
    while(ii >> c) {
        if(c != '{') {
            if(c == '}') throw runtime_error("");
            formatted += c;
        }
        else {
            long n;
            ii >> n;

            if(arguments.size() < n + 1) throw runtime_error("");
            formatted += arguments[n];

            ii >> c;
            if(c != '}') throw runtime_error("");
        }
    }
    return formatted;
}