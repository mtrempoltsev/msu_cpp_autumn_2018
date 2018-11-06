class List{
private:
    struct Node{
        Node() : next(nullptr), elem(0){}
        Node *next;
        int elem;
    };
    Node *head;
    Node *tail;
public:
    size_t size;
    List() : head(nullptr), tail(nullptr), size(0){}
    List(const List &a) : head(nullptr), tail(nullptr), size(0){
        for(auto tmp = a.head; tmp; tmp = tmp->next){
            this->push_back(tmp->elem);
        }
    }
    List(int64_t a) : head(nullptr), tail(nullptr), size(0){
        push_back(a);
    }
    List& operator=(const List& x){
        if(this == &x){
            return *this;
        }
        while(size != 0)
            pop_front();
        for(auto tmp = x.head; tmp; tmp = tmp->next){
            push_back(tmp->elem);
        }
        return *this;
    }
    ~List(){
        while(head){
            tail = head->next;
            delete head;
            head = tail;
        }
    }
    List operator+(const List &x) const{
        List tmp;
        auto tmp1 = head;
        auto tmp2 = x.head;
        int digit = 0;
        while(tmp1 && tmp2){
            tmp.push_back((tmp1->elem + tmp2->elem + digit) % 10);
            digit = (tmp1->elem + tmp2->elem + digit) / 10;
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }
        if(tmp1){
            tmp.push_back(tmp1->elem + digit);
            digit = 0;
            tmp1 = tmp1->next;
        }
        if(tmp2){
            tmp.push_back(tmp2->elem + digit);
            digit = 0;
            tmp2 = tmp2->next;
        }
        if(digit) tmp.push_back(digit);
        return tmp;
    }
    List operator-(const List &x) const{
        List tmp;
        auto tmp1 = head;
        auto tmp2 = x.head;
        int digit = 0;
        int buf;
        while(tmp1 || tmp2){
            if(tmp1 && tmp2){
                buf = 10 + tmp1->elem - tmp2->elem - digit;
                tmp1 = tmp1->next;
                tmp2 = tmp2->next;
            }else{
                if(tmp1){
                    buf = 10 + tmp1->elem - digit;
                    tmp1 = tmp1->next;
                }else{
                    buf = 10 + tmp2->elem - digit;
                    tmp2 = tmp2->next;
                }
            }
            digit = (buf >= 10 ? 0 : 1);
            tmp.push_back(buf % 10);
        }
        return tmp;
    }
    Node *begin() const{
        return head;
    }
    Node *end() const{
        return tail;
    }
    List inverse() const{
        List tmp;
        for(auto i = head; i; i = i->next){
            tmp.push_front(i->elem);
        }
        while(tmp.size > 1 && tmp.begin()->elem == 0) tmp.pop_front();
        return tmp;
    }
    void pop_front(){
        auto tmp = head->next;
        delete head;
        head = tmp;
        size--;
    }
    void push_back(int val){
        Node *Temp = new Node;
        Temp->elem = val;
        size++;
        if(!head){
            head = Temp;
            tail = Temp;
            return;
        }
        tail->next = Temp;
        tail = Temp;
    }
    void push_front(int val){
        Node* Temp;
        Temp = new Node;
        Temp->elem = val;
        Temp->next = head;
        head = Temp;
        size++;
    }
};

class BigInt{
    List num;
    bool sign;
    bool abs(const BigInt &x) const{
        if(this->num.size != x.num.size)
            return this->num.size > x.num.size;
        List tmp1 = this->num.inverse();
        List tmp2 = x.num.inverse();
        for(auto ptr1 = tmp1.begin(),
            ptr2 = tmp2.begin();
            ptr1;
            ptr1 = ptr1->next, ptr2 = ptr2->next){
            if(ptr1->elem > ptr2->elem) return true;
            if(ptr1->elem < ptr2->elem) return false;
        }
        return false;
    }
public:
    BigInt() : num(0), sign(true){}
    BigInt(const BigInt &tmp) : num(tmp.num), sign(tmp.sign){}
    BigInt(int64_t x) : num(), sign(x>=0){
        if(x == 0) num.push_back(0);
        x = x > 0 ? x : -x;
        for(int64_t tmp = x; tmp != 0; tmp/=10)
            num.push_back(tmp % 10);
    }
    BigInt(const List &x, bool sign = true) : num(), sign(sign){
        for(auto tmp = x.begin(); tmp; tmp = tmp->next){
            this->num.push_back(tmp->elem);
        }
    }
    BigInt& operator=(const BigInt& x){
        if(this == &x){
            return *this;
        }
        num = x.num;
        sign = x.sign;
        return *this;
    }
    BigInt operator+(const BigInt &x) const{
        if(sign != x.sign){
            if(abs(x)){
                return BigInt(num - x.num, sign);
            }else{
                return BigInt(x.num - num, x.sign);
            }
        }
        return BigInt(num + x.num, x.sign);
    }
    BigInt operator+(int64_t x) const{
        BigInt buf(x);
        return (*this+buf);
    }
    BigInt operator-(const BigInt &x) const{
        if((sign || x.sign) && !(sign && x.sign))
            return BigInt(num + x.num, sign);
        if(abs(x))
            return BigInt(num - x.num, sign);
        return BigInt(x.num - num, !(sign && x.sign));
    }
    BigInt operator-() const{
        return BigInt(num, !sign);
    }
    bool operator>(const BigInt &x) const{
        if(this->sign != x.sign)
            return this->sign;
        bool buf = abs(x);
        if(x.sign) return buf;
        return !buf;
    }
    bool operator==(const BigInt &x) const{
        if(this->num.size != x.num.size)
            return false;
        List tmp1 = this->num.inverse();
        List tmp2 = x.num.inverse();
        for(auto ptr1 = tmp1.begin(),
            ptr2 = tmp2.begin();
            ptr1;
            ptr1 = ptr1->next, ptr2 = ptr2->next){
            if(ptr1->elem != ptr2->elem) return false;
        }
        return true;
    }
    bool operator!=(const BigInt &x) const{
        return !(*this == x);
    }
    bool operator>=(const BigInt &x) const{
        return (*this == x) || (*this > x);
    }
    bool operator<(const BigInt &x) const{
        return !(*this >= x);
    }
    bool operator<=(const BigInt &x) const{
        return !(*this > x);
    }
    friend std::ostream & operator<<(std::ostream& out, const BigInt& buf);
};

std::ostream& operator<<(std::ostream& out, const BigInt& buf){
    List tmp = buf.num.inverse();
    if(!buf.sign && tmp.begin()->elem != 0) out << "-";
    for(auto ptr = tmp.begin(); ptr; ptr=ptr->next){
        out << ptr->elem;
    }
    return out;
}
