#include <iostream>
#include <exception>
#include <string>

class BigInt {
    static const int cell_max = 1000 * 1000 * 1000;
    static const size_t cell_sz = 9;
    
    size_t size;
    int *num;
    int sign;
    
    std::string trim_zeros(const std::string& str0) const
    {
        std::string str = str0;
        if (str[0] == '0' && str.size() != 1) {
            str = trim_zeros(str.substr(1, str.size() - 1));
        }
        return str;
    }
    std::string normalize_str(const std::string& str0) const
    {
        std::string str = str0;
        while (str.size() % cell_sz != 0) {
            str = "0" + str;
        }
        return str;
    }
    static std::pair<int, int> half_add(int x1, int x2, int cf = 0)
    {
        int tmp = x1 + x2 + cf;
        if (tmp < cell_max) {
            cf = 0;
        } else {
            cf = 1;
            tmp %= cell_max;
        }
        return std::pair<int, int>(tmp, cf);
    }
    static BigInt add_two_positives(const BigInt& x1, const BigInt& x2)
    {
        if (x2.size > x1.size) {
            return add_two_positives(x2, x1);
        }
        BigInt bigint(x1);
        int cf = 0;
        size_t s1 = x1.size - 1;
        size_t s2 = x2.size - 1;
        std::pair<int, int> tmp(0, cf);
        
        while (s2 != static_cast<size_t>(-1)) {
            tmp = half_add(x1.num[s1], x2.num[s2], tmp.second);
            bigint.num[s1] = tmp.first;
            --s1;
            --s2;
        }
        while (s1 != static_cast<size_t>(-1)) {
            tmp = half_add(x1.num[s1], 0, tmp.second);
            bigint.num[s1] = tmp.first;
            --s1;
        }
        if (tmp.second == 1) {
            int *tmp_ptr = new int[bigint.size + 1];
            tmp_ptr[0] = 1;
            for (size_t i = 0; i < bigint.size; ++i) {
                tmp_ptr[i + 1] = bigint.num[i];
            }
            delete[] bigint.num;
            bigint.size += 1;
            bigint.num = tmp_ptr;
        }
        return bigint;
    }
    static std::pair<int, int> half_sub(int x1, int x2, int cf = 0)
    {
        int tmp = x1 - x2 - cf;
        if (tmp >= 0) {
            cf = 0;
        } else {
            cf = 1;
            tmp += cell_max;
        }
        return std::pair<int, int>(tmp, cf);
    }
    static BigInt sub_two_positives(const BigInt& x1, const BigInt& x2)
    {
        if (x2 > x1) {
            return -sub_two_positives(x2, x1);
        }
        BigInt bigint(x1);
        int cf = 0;
        size_t s1 = x1.size - 1;
        size_t s2 = x2.size - 1;
        std::pair<int, int> tmp(0, cf);
        
        while (s2 != static_cast<size_t>(-1)) {
            tmp = half_sub(x1.num[s1], x2.num[s2], tmp.second);
            bigint.num[s1] = tmp.first;
            --s1;
            --s2;
        }
        while (s1 != static_cast<size_t>(-1)) {
            tmp = half_sub(x1.num[s1], 0, tmp.second);
            bigint.num[s1] = tmp.first;
            --s1;
        }
        size_t zeros = 0;
        for (zeros = 0; zeros < bigint.size; ++zeros) {
            if (bigint.num[zeros] != 0) {
                break;
            }
            int *tmp_ptr = new int[bigint.size - zeros];
            for (size_t i = 0; i < bigint.size - zeros; ++i) {
                tmp_ptr[i] = bigint.num[i + zeros];
            }
            delete[] bigint.num;
            bigint.size -= zeros;
            bigint.num = tmp_ptr;
        }
        return bigint;
    }
    
public:
    BigInt() : BigInt("0")
    {
    }
    BigInt(int64_t x) : BigInt(std::to_string(x))
    {
    }
    BigInt(const std::string& str0)
    {
        std::string str = str0;
        sign = 1;
        if (str0[0] == '-') {
            sign = -1;
            str = str.substr(1, str.size() - 1);
        }
        str = trim_zeros(str);
        str = normalize_str(str);
        
        size = str.size() / cell_sz;
        num = new int[size];
        
        for (size_t i = 0; i < str.size() / cell_sz; ++i) {
            std::string tmp = str.substr(i * cell_sz, cell_sz);
            num[i] = std::atoi(tmp.c_str());
        }
    }
    BigInt(const BigInt& x)
    {
        sign = x.sign;
        size = x.size;
        num = new int[size];
        for (size_t i = 0; i < size; ++i) {
            num[i] = x.num[i];
        }
    }
    ~BigInt()
    {
        delete[] num;
    }
    
    BigInt& operator= (const BigInt& x)
    {
        sign = x.sign;
        size = x.size;
        int *tmp_ptr = new int[size];
        delete[] num;
        num = tmp_ptr;
        for (size_t i = 0; i < size; ++i) {
            num[i] = x.num[i];
        }
        return *this;
    }
    
    BigInt abs() const
    {
        BigInt tmp(*this);
        tmp.sign = 1;
        return tmp;
    }
    BigInt operator+ () const
    {
        BigInt tmp(*this);
        tmp.sign *= 1;
        return tmp;
    }
    BigInt operator- () const
    {
        BigInt tmp(*this);
        tmp.sign *= -1;
        return tmp;
    }

    
//    a + b == a + b
//    a + -b == a - |b|
//    -a + b == b - |a|
//    -a + -b == -(|a| + |a|)
    friend BigInt operator+ (const BigInt& x1, const BigInt& x2)
    {
        if (x1.sign == x2.sign) {
            if (x1.sign == 1) {
                return add_two_positives(x1, x2);
            } else {
                return -(x1.abs() + x2.abs());
            }
        } else {
            if (x1.sign == 1) {
                return x1 - x2.abs();
            } else {
                return x2 - x1.abs();
            }
        }
    }
    
//    a - b == a - b
//    a - -b == a + |b|
//    -a - b == -(|a| + b)
//    -a - -b == -(|a| - |b|)
    friend BigInt operator- (const BigInt& x1, const BigInt& x2)
    {
        if (x1.sign == x2.sign) {
            if (x1.sign == 1) {
                return sub_two_positives(x1, x2);
            } else {
                return -(x1.abs() - x2.abs());
            }
        } else {
            if (x1.sign == 1) {
                return x1 + x2.abs();
            } else {
                return -(x1.abs() + x2);
            }
        }
    }
    
    friend bool operator== (const BigInt& x1, const BigInt& x2)
    {
        if ((x1.num[0] == 0) && (x2.num[0] == 0)) {
            return true;
        }
        if (x1.size != x2.size || x1.sign != x2.sign) {
            return false;
        }
        for (size_t i = 0; i < x1.size; ++i) {
            if (x1.num[i] != x2.num[i]) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!= (const BigInt& x1, const BigInt& x2)
    {
        return !(x1 == x2);
    }
    friend bool operator>= (const BigInt& x1, const BigInt& x2)
    {
        if (x1.sign > x2.sign) { // '+' > '-'
            return true;
        } else if (x1.sign < x2.sign) {
            return false;
        }
        
        if (x1.size != x2.size) {
            if (x1.sign == 1) { // '+' ? '+'
                return x1.size > x2.size ? true : false;
            } else { // '-' ? '-'
                return x1.size < x2.size ? true : false;
            }
        } else {
            if (x1.sign == 1) {
                for (size_t i = 0; i < x1.size; ++i) {
                    if (x1.num[i] < x2.num[i]) {
                        return false;
                    } else if (x1.num[i] > x2.num[i]) {
                        return true;
                    }
                }
                return true;
            } else {
                for (size_t i = 0; i < x1.size; ++i) {
                    if (x1.num[i] > x2.num[i]) {
                        return false;
                    } else if (x1.num[i] < x2.num[i]) {
                        return true;
                    }
                }
                return true;
            }
        }
    }
    friend bool operator<= (const BigInt& x1, const BigInt& x2)
    {
        if (x1.sign < x2.sign) { // '+' > '-'
            return true;
        } else if (x1.sign > x2.sign) {
            return false;
        }
        
        if (x1.size != x2.size) {
            if (x1.sign == 1) { // '+' ? '+'
                return x1.size < x2.size ? true : false;
            } else { // '-' ? '-'
                return x1.size > x2.size ? true : false;
            }
        } else {
            if (x1.sign == 1) {
                for (size_t i = 0; i < x1.size; ++i) {
                    if (x1.num[i] > x2.num[i]) {
                        return false;
                    } else if (x1.num[i] < x2.num[i]) {
                        return true;
                    }
                }
                return true;
            } else {
                for (size_t i = 0; i < x1.size; ++i) {
                    if (x1.num[i] < x2.num[i]) {
                        return false;
                    } else if (x1.num[i] > x2.num[i]) {
                        return true;
                    }
                }
                return true;
            }
        }
    }
    friend bool operator> (const BigInt& x1, const BigInt& x2)
    {
        return !(x1 <= x2);
    }
    friend bool operator< (const BigInt& x1, const BigInt& x2)
    {
        return !(x1 >= x2);
    }

    friend std::ostream& operator<< (std::ostream& out, const BigInt& bigint)
    {
        if (bigint.sign == -1 && bigint != BigInt(0)) {
            out << '-';
        }
        for (size_t i = 0; i < bigint.size; ++i) {
            std::string tmp = std::to_string(bigint.num[i]);
            if (i != 0) {
                tmp = bigint.normalize_str(tmp);
            }
            out << tmp;
        }
        return out;
    }
};

//#if !defined tmode
//int main()
//{
//    std::string str, str1;
//    std::cin >> str;
//    std::cin >> str1;
//    while (str != "quit" && str != "q") {
//        BigInt x(str);
//        BigInt y(str1);
//
//        std::cout << "x: " << x << "\n" << "y: " << y << "\n";
//        std::cout << "== " << (x == y) << std::endl;
//        std::cout << "!= " << (x != y) << std::endl;
//        std::cout << "<= " << (x <= y) << std::endl;
//        std::cout << ">= " << (x >= y) << std::endl;
//        std::cout << " < " << (x < y) << std::endl;
//        std::cout << " > " << (x > y) << std::endl;
//
//        std::cout << "x + y : " << (x + y) << std::endl;
//        std::cout << "x - y : " << (x - y) << std::endl;
//        std::cin >> str;
//        std::cin >> str1;
//    }
//    return 0;
//}
//#endif
