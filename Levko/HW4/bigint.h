#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

class Node {
public:
    Node(): next(nullptr), elem(0){}
    Node* next;
    int elem;
};

class ArrayList {
    using Size = size_t;
public:    
    Node* head = nullptr;
    Node* tail = nullptr;
    Size size;
    
    ArrayList() : head(), tail(), size(0){}
            
    ArrayList(const ArrayList &newArrayList) 
    : head(nullptr), 
    tail(nullptr), 
    size(0)
        {
            for(auto iter = newArrayList.head; iter; iter = iter->next) {
                this->push_back(iter->elem);
            }
        }
    
    ArrayList(int a) : head(nullptr), tail(nullptr), size(0)
        {
            push_back(a);
        }
        
        ArrayList& operator=(const ArrayList& other) {
        
        if(this == &other) {
            return *this;
        }
        
        while(size != 0) pop_front();
        
        for(auto iter = other.head; iter; iter = iter->next) {
            push_back(iter->elem);
        }
        return *this;
    }
    
    ~ArrayList() {
        while(head) {
            tail = head->next;
            delete head;
            head = tail;
        }
    }
    

    ArrayList operator+(const ArrayList &other) const {
    
        ArrayList tmp;
        auto tmp1 = head;
        auto tmp2 = other.head;
        int digit = 0;
        
        while(tmp1 && tmp2) {
            tmp.push_back((tmp1->elem + tmp2->elem + digit) % 10);
            digit = (tmp1->elem + tmp2->elem + digit) / 10;
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }
        
        while(tmp1) {
            tmp.push_back((tmp1->elem + digit) % 10);
            digit = (tmp1->elem + digit) / 10;
            tmp1 = tmp1->next;
        }
        
        while(tmp2) {
            tmp.push_back((tmp2->elem + digit) % 10);
            digit = (tmp2->elem + digit) / 10;
            tmp2 = tmp2->next;
        }
        
        if(digit) tmp.push_back(digit);
        
        return tmp;
    }
    

    ArrayList operator-(const ArrayList &other) const{
        ArrayList tmp;
        auto tmp1 = head;
        auto tmp2 = other.head;
        int digit = 0;
        int buf;
        
        while(tmp1 || tmp2) {
            
            if(tmp1 && tmp2) {
                
                buf = 10 + tmp1->elem - tmp2->elem - digit;
                tmp1 = tmp1->next;
                tmp2 = tmp2->next;
                
            } else {
                
                if(tmp1) {
                    buf = 10 + tmp1->elem - digit;
                    tmp1 = tmp1->next;
                } else {
                    buf = 10 + tmp2->elem - digit;
                    tmp2 = tmp2->next;
                }
            }
            digit = (buf >= 10 ? 0 : 1);
            tmp.push_back(buf % 10);
        }
        return tmp;
    }
    
    Node *begin() const {
        return head;
    }
    
    Node *end() const {
        return tail;
    }
    
    ArrayList inverse() const {
        ArrayList tmp;
        for(auto iter = head; iter; iter = iter->next) {
            tmp.push_front(iter->elem);
        }
        while(tmp.size > 1 && tmp.begin()->elem == 0) tmp.pop_front();
        return tmp;
    }
    
    void pop_front() {
        auto tmp = head->next;
        delete head;
        head = tmp;
        size--;
    }
    
    void push_back(int val) {
        Node *Temp = new Node;
        Temp->elem = val;
        size++;
        if(!head) {
            head = Temp;
            tail = Temp;
            return;
        }
        tail->next = Temp;
        tail = Temp;
    }
    
    void push_front(int val) {
        Node* Temp;
        Temp = new Node;
        Temp->elem = val;
        Temp->next = head;
        head = Temp;
        size++;
    }
};


class BigInt 
{
    using Int = int64_t;
    
	ArrayList number;
	
public:
    
	bool isNegative;

	BigInt() :number(0), isNegative(false){}
        
    BigInt(const BigInt &tmp) : number(tmp.number), isNegative(tmp.isNegative) {}
	
	BigInt(Int i) : number(), isNegative(i < 0)
        {
            if(i == 0) number.push_back(0);
            i = i > 0 ? i : -i;
            for(auto tmp = i; tmp != 0; tmp/=10)
                number.push_back(tmp % 10);
        }
    
    BigInt(const ArrayList &x, bool isNegative = false) : number(), isNegative(isNegative) {
        for(auto tmp = x.begin(); tmp; tmp = tmp->next){
            this->number.push_back(tmp->elem);
        }
    }
        
        
    bool abs(const BigInt &x) const {
        if(this->number.size != x.number.size)
            return this->number.size > x.number.size;
        ArrayList tmp1 = this->number.inverse();
        ArrayList tmp2 = x.number.inverse();
        for(auto ptr1 = tmp1.begin(),
            ptr2 = tmp2.begin();
            ptr1;
            ptr1 = ptr1->next, ptr2 = ptr2->next){
            if(ptr1->elem > ptr2->elem) return true;
            if(ptr1->elem < ptr2->elem) return false;
        }
        return true;
    }
    
    friend std::ostream & operator<<(std::ostream& out, const BigInt& buf);
	
	const BigInt operator+() const {
		return BigInt(*this);
	}
	
	const BigInt operator-() const {
        return BigInt(number, !isNegative);
	}
	
	bool operator==(const BigInt& other) const {
        ArrayList tmp1 = this->number.inverse();
        ArrayList tmp2 = other.number.inverse();
        if(tmp1.size != tmp2.size)
            return false;
        for(auto ptr1 = tmp1.begin(),
            ptr2 = tmp2.begin();
            ptr1;
            ptr1 = ptr1->next, ptr2 = ptr2->next) {
            if(ptr1->elem != ptr2->elem) return false;
        }
        return true;
	}
	
	BigInt& operator=(const BigInt& other){
        if(this == &other) return *this;
        number = other.number;
        isNegative = other.isNegative;
        return *this;
    }
	
	bool operator<(const BigInt& other) const {
		if(this->isNegative != other.isNegative)
            return this->isNegative;
        bool buf = abs(other);
        if(other.isNegative) return buf;
        return !buf;
	}
	
	bool operator!=(const BigInt& other) const {
		return !(*this == other);
	}
	
	bool operator<=(const BigInt& other) const {
		return (operator < (other) || operator == (other));
	}
	
	bool operator>(const BigInt& other) const {
		return !(operator <= (other));
	}
	
	bool operator>=(const BigInt& other) const {
		return ((*this == other) || (*this > other));
	}
	
	const BigInt operator+(const BigInt& other) const {
		
        if(isNegative != other.isNegative) {
            if(abs(other))return BigInt(number - other.number, isNegative);
            else return BigInt(other.number - number, other.isNegative);
        }
        
        return BigInt(number + other.number, isNegative);
	}
	
	BigInt operator+(int64_t &x) const {
        BigInt buf(x);
        return (*this + buf);
    }
	
	BigInt& operator+=(const BigInt& other) {
		return *this = (*this + other);
	}
	
	const BigInt operator-(const BigInt& other) const {
        if((isNegative || other.isNegative) && !(isNegative && other.isNegative)) 
            return BigInt(number + other.number, isNegative);
        if(abs(other)) return BigInt(number - other.number, isNegative);
        return BigInt(other.number - number, !(isNegative && other.isNegative));
	}
	
	BigInt& operator-=(const BigInt& other) {
		return *this = (*this - other);
	}
};

std::ostream& operator<<(std::ostream& out, const BigInt& buf){
    ArrayList tmp = buf.number.inverse();
    if(buf.isNegative && tmp.begin()->elem != 0) out << "-";
    for(auto ptr = tmp.begin(); ptr; ptr=ptr->next){
        out << ptr->elem;
    }
    return out;
}
