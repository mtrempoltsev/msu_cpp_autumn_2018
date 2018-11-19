#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

int max_number(const string& str, vector<string>& substrings){
    int max_num = -1;
    int now;
    bool in_brackets = false;
    string help = "";
    for (char c : str){
        if (c == '{'){
            if (in_brackets)
                throw runtime_error("");
            in_brackets = true;
            if (help != "")
                substrings.push_back(help);
            help = "";
        } else {
            if (c == '}'){
                if (!in_brackets)
                    throw runtime_error("");
                in_brackets = false;
                now = stoull(help);
                if (now > max_num)
                    max_num = now;
                substrings.push_back("{" + help);
                help = "";
            } else {
                if ((in_brackets) && ((c < 48) || (c > 57)))
                    throw runtime_error("");
                help += c;
            }
        }
    }
    if (in_brackets)
        throw runtime_error("");
    if (help != "")
	{
		substrings.push_back(help);
	}
	if (max_num != -1)
        max_num++;
	return max_num;
}
void build_replacement(vector<string>& replacement, const int& max_num)
{
	throw runtime_error("");
}

template <class T, class... Args>
void build_replacement(vector<string>& replacement, const int& max_num, T value, Args&&... args)
{
    stringstream res;
	res << value;
	replacement.push_back(res.str());
	if (replacement.size() < max_num)
        build_replacement(replacement, max_num, forward<Args>(args)...);
}

template <class... Args>
string format(const string& s, Args&&... args){
    vector<string> substrings;
    int max_num = max_number(s, substrings);
	if (max_num == -1)
		return s;
    vector<string> replacement;
	build_replacement(replacement, max_num, forward<Args>(args)...);
	string res = "";
	for (int i = 0; i < substrings.size(); i++){
        if (substrings[i][0] != '{')
            res += substrings[i];
        else{
            res += replacement[stoull(substrings[i].substr(1, substrings[i].size() - 1))];
        }
	}
	return res;
}
