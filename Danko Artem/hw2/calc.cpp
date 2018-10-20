#include <iostream>
#include <stack>

//it's really dangerous variant using stack
//other is better(using tree)

class MyCalc {
    union Elem {
        char s;
        int64_t n;
        Elem(int64_t n1){
            n = n1;
        }
        Elem(char i){
            s = i;
        }
    };
    char* s;

public:
    MyCalc(char* a){
        s = a;//really we should copy this string, but in this local task...
    }
    int64_t solve(){
        if ( s == nullptr )
            throw std::invalid_argument("wtf");
        std::stack<Elem*> stack_of_elems;
        stack_of_elems.push(new Elem('+'));
        try {
            stack_of_elems.push(get_num());
        } catch (std::out_of_range&) {
            throw std::invalid_argument("empty imput");
        }
        while(true){
            Elem* sim;
            Elem* num;
            try {
                sim = get_sim();
            } catch (std::out_of_range&) {
                break;
            }
            try {
                num = get_num();
            } catch (std::out_of_range&) {
                throw std::invalid_argument("strange last sim");
            }
            if (sim->s == '*' || sim->s == '/') {
                Elem* temp = stack_of_elems.top();stack_of_elems.pop();
                stack_of_elems.push(multiplier(temp, sim, num));
            } else if (sim->s == '-' || sim->s == '+') {
                stack_of_elems.push(sim);
                stack_of_elems.push(num);
            } else throw std::invalid_argument("strange simbol");
        }
        stack_of_elems.push(new Elem(int64_t(0)));
        while(stack_of_elems.size() != 1){
            Elem* num1 = stack_of_elems.top();stack_of_elems.pop();
            Elem* num2 = stack_of_elems.top();stack_of_elems.pop();
            Elem* sim1 = stack_of_elems.top();stack_of_elems.pop();
            stack_of_elems.push(multiplier(num1, sim1, num2));
        }
        int64_t answ = stack_of_elems.top()->n;
        free(stack_of_elems.top());
        return answ;
    }
private:
    void skip_spaces() {
        //miss spaces
        while (*s != '\0' && isspace(int(*s))){s++;}
        if (*s == '\0')
            throw std::out_of_range("end_of_string");
    }
    Elem* get_num(){
        int um_m = 1;
        int64_t answ = 0;
        //check unary minus
        skip_spaces();
        if (*s == '-') {
            um_m = -1;
            s++;
        }
        //get number
        skip_spaces();
        while (*s != '\0' && isdigit(*s)){
            answ = answ * 10 + *s - '0';
            s++;
        }
        return new Elem(answ * um_m);
    }
    Elem* get_sim() {
        skip_spaces();
        return new Elem(*s++);
    }
    Elem* multiplier(Elem* a, Elem* s, Elem* b) {
        int64_t a_v = a->n, b_v = b->n;
        char sim = s->s;
        free(a), free(b), free(s);
        switch(sim){
            case '*': return new Elem (a_v * b_v);
            case '-': return new Elem (a_v - b_v);
            case '+': return new Elem (a_v + b_v);
            case '/': if (b_v == 0) throw std::invalid_argument("zero div");
                return new Elem (a_v / b_v);
        }
        return new Elem ('-');
    }
};

int main(int argc, char* argv[])
{
    MyCalc a = MyCalc(argv[1]);
    try {
        if (argc != 2)
            throw std::invalid_argument("count fault");
        std::cout << a.solve() << std::endl;
    } catch(std::exception&){
        std::cout << "error" << std::endl;
        return 1;
    }
    return 0;
}

