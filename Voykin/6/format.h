#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

template <class T>
string to_string (T&& s)
{
    stringstream in_;
    in_ << s;
    return in_.str();
}

int max_number_and_division_token(const string& str, vector<string>& tokens)
{
    int max_number = -1;
    int now_number;
    bool open_bracket = false;
    string in_bracket = "";
    for (char x : str)
    {
        if (x == '{')
        {
            if (open_bracket)
            {
                throw runtime_error("");
            }
            if (in_bracket != "")
            {
                tokens.push_back(in_bracket);
                in_bracket = "";
            }
            open_bracket = true;
        }
        else
        {
            if (x == '}')
            {
                if (!open_bracket)
                {
                    throw runtime_error("");
                }
                now_number = stoull(in_bracket);
                max_number = max(max_number, now_number);
                tokens.push_back("{" + in_bracket + "}");
                open_bracket = false;
                in_bracket = "";
            }
            else
            {
                if ((open_bracket) && !((x > 47) && (x < 58)))
                {
                    throw runtime_error("");
                }
                in_bracket += x;
            }
        }
    }
    if (open_bracket)
    {
        throw runtime_error("");
    }
    if (in_bracket != "")
    {
        tokens.push_back(in_bracket);
    }
    return max_number;
}

template <class... Args>
string format(const string& str, Args&&... args)
{
    vector<string> tokens;
    vector<string> args_to_string{to_string(forward<Args>(args))...};
    string new_format = "";
    int max_number_ = max_number_and_division_token(str, tokens);
    if (args_to_string.size() != max_number_ + 1)
    {
        throw runtime_error("");
    }
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i][0] == '{')
        {
            new_format += args_to_string[stoull(tokens[i].substr(1, tokens[i].size() - 2))];
        }
        else
        {
            new_format += tokens[i];
        }
    }
    return new_format;
}
