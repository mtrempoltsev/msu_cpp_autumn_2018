#include <iostream>
#include <cinttypes>
#include <ostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
using namespace std;
enum type_of_lex
{
    LEX_NULL,
    LEX_FIN,
    LEX_INTEGER,
    LEX_SLASH,
    LEX_STAR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_SPACE,
    LEX_UNARY
};
class Lex
{
    type_of_lex type;
    int64_t value;
public:
    Lex(type_of_lex t = LEX_NULL, int64_t v = 0)
    {
        type = t;
        value = v;
    }
    Lex(const Lex &l)
    {
        type = l.type;
        value = l.value;
    }
    type_of_lex get_type() const
    {
        return  type;
    }
    void set_type(type_of_lex t)
    {
        type = t;
    }
    int64_t get_value() const
    {
        return value;
    }
    void set_value(int64_t v)
    {
        value = v;
    }

    friend std::ostream& operator << (std::ostream& s, const Lex &l)//debug func
    {
        s << "{ TYPE = <";
        switch(l.get_type())
        {
            case LEX_NULL:
                s << "LEX_NULL";
                break;

            case LEX_FIN:
                s << "LEX_FIN";
                break;

            case LEX_INTEGER:
                s << "LEX_INTEGER";
                break;

            case LEX_SLASH:
                s << "LEX_SLASH";
                break;

            case LEX_STAR:
                s << "LEX_STAR";
                break;

            case LEX_PLUS:
                s << "LEX_PLUS";
                break;

            case LEX_MINUS:
                s << "LEX_MINUS";
                break;

            case LEX_UNARY:
                s << "LEX_UNARY";
                break;

            case LEX_SPACE:
                s << "LEX_SPACE";
                break;
        }
        s << ">";
        if(l.get_type() == LEX_INTEGER)
            s << ", VALUE = <" << l.get_value() << ">;";
        return s << " }";
    }
};
class sin_analizator
{
    string inp;
    vector<Lex> poliz_vector;
    stack<Lex> poliz_stack;
    type_of_lex curr_lex;
    int64_t curr_value;
    char curr_char;
    unordered_map<char, type_of_lex> TD =
    {
        {'/', LEX_SLASH},
        {'*', LEX_STAR},
        {'+', LEX_PLUS},
        {'-', LEX_MINUS},
        {'\0',LEX_FIN}
    };
    unordered_map<type_of_lex, int> privilege =
    {
        {LEX_PLUS,  1},
        {LEX_MINUS, 1},
        {LEX_STAR,  2},
        {LEX_SLASH, 2},
        {LEX_UNARY, 3}
    };
    void add_to_poliz()
    {
        if(curr_lex == LEX_INTEGER)
            poliz_vector.emplace_back(curr_lex, curr_value);
        else if(curr_lex == LEX_FIN)
        {
            while(!poliz_stack.empty())
            {
                poliz_vector.push_back(poliz_stack.top());
                poliz_stack.pop();
            }
            poliz_vector.emplace_back(curr_lex);
        }
        else if (poliz_stack.empty())
            poliz_stack.emplace(curr_lex);
        else if(curr_lex == LEX_UNARY)
        {
            if(privilege[poliz_stack.top().get_type()] > privilege[curr_lex])
            {
                while(!poliz_stack.empty() && privilege[poliz_stack.top().get_type()] > privilege[curr_lex])
                {
                    poliz_vector.push_back(poliz_stack.top());
                    poliz_stack.pop();
                }
                poliz_stack.emplace(curr_lex);
            }
            else
                poliz_stack.emplace(curr_lex);
        }
        else
        {
            if(privilege[poliz_stack.top().get_type()] >= privilege[curr_lex])
            {
                while(!poliz_stack.empty() && privilege[poliz_stack.top().get_type()] >= privilege[curr_lex])
                {
                    poliz_vector.push_back(poliz_stack.top());
                    poliz_stack.pop();
                }
                poliz_stack.emplace(curr_lex);
            }
            else
                poliz_stack.emplace(curr_lex);
        }
    }
    int is_digit(char c)
    {
        if(c < '0' || c > '9')
            return  -1;
        return c - '0';
    }
    char gc()
    {
        static size_t i = 0;
        while(inp[i] == ' ' || inp[i] == '\n' || inp[i] == '\t' || inp[i] == '\r')
            ++i;
        if(inp[i] == '\0')
            return '\0';
        return inp[i++];
    }
    bool unary_m()
    {
        if(TD.find(curr_char) != TD.end())
        {
            if((curr_lex = TD[curr_char]) != LEX_MINUS)
                return  false;
            else
            {
                curr_char = gc();
                curr_lex = LEX_UNARY;
                return unary_p();
            }
        }
        else
            return digit_s();

    }
    bool unary_p()
    {
        if(TD.find(curr_char) != TD.end())
        {
            if((curr_lex = TD[curr_char]) != LEX_MINUS)
                return  false;
            else
            {
                curr_char = gc();
                curr_lex = LEX_NULL;
                return unary_m();
            }
        }
        else
        {
            add_to_poliz();
            return digit_s();
        }
    }
    bool digit_s()
    {
        if((curr_value = is_digit(curr_char)) != -1)
        {
            curr_char = gc();
            curr_lex = LEX_INTEGER;
            return digit();
        }
        else
            return false;
    }
    bool digit()
    {
        int v = 0;
        if(TD.find(curr_char) != TD.end())
        {
            add_to_poliz();
            curr_value = 0;
            curr_lex = TD[curr_char];
            add_to_poliz();
            if(curr_lex == LEX_FIN)
                return true;//END
            else
            {
                curr_char = gc();
                return unary_m();
            }
        }
        else if((v = is_digit(curr_char)) != -1)
        {
            curr_value = curr_value * 10 + v;
            curr_char = gc();
            return digit();
        }
        else
            return false;
    }
public:
    sin_analizator(string s)
    {
        inp = s;
        curr_value = 0;
        curr_lex = LEX_NULL;
        curr_char = '\0';
    }
    bool parse()
    {
        if((curr_char = gc()) == '\0')
        {
            curr_lex = LEX_FIN;
            add_to_poliz();
            return true;//END
        }
        return unary_m();
    }
    void print()//debug func
    {
        for(auto const& i: poliz_vector)
            cout << i << endl;
    }
    int64_t execute()
    {
        int64_t top;
        for(auto const& i: poliz_vector)
        {
            switch (i.get_type())
            {
            case LEX_INTEGER:
                poliz_stack.push(i);
                break;

            case LEX_FIN:
                return poliz_stack.top().get_value();

            case LEX_UNARY:
                poliz_stack.top().set_value(-poliz_stack.top().get_value());
                break;

            case LEX_MINUS:
                top = poliz_stack.top().get_value();
                poliz_stack.pop();
                poliz_stack.top().set_value(poliz_stack.top().get_value() - top);
                break;

            case LEX_PLUS:
                top = poliz_stack.top().get_value();
                poliz_stack.pop();
                poliz_stack.top().set_value(poliz_stack.top().get_value() + top);
                break;

            case LEX_STAR:
                top = poliz_stack.top().get_value();
                poliz_stack.pop();
                poliz_stack.top().set_value(poliz_stack.top().get_value() * top);
                break;

            case LEX_SLASH:
                if((top = poliz_stack.top().get_value()) == 0)
                {
                	cout << "error" << endl;
                	exit(1);
                }
                poliz_stack.pop();
                poliz_stack.top().set_value(poliz_stack.top().get_value() / top);
                break;

            default:
                cout << "smth bad happend" << endl;
            }
        }
        return 0;
    }
};


int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		cout << "error" << endl;
		return 1;
	}
	sin_analizator s(argv[1]);
	if(!s.parse())
    {
        cout << "error" << endl;
        return 1;
    }
    cout << s.execute() << endl;
    return 0;
}

