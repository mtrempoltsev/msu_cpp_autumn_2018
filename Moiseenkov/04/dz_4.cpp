#include <iostream>
#include <cstring>

class BigInt {

    size_t capacity = 8;

    size_t size = 0;
    char* data = nullptr;
    bool sign = false;

    
    BigInt UpdateSize()
    {
        while (size > 1 && data[size - 1] == 0)
            size--;

        if (size == 1 && data[0] == 0)
            sign = false;
        
        return *this;
    }
    
    
    
    void r_add(const BigInt& number, BigInt& result) const
    {
        char extra = 0;

        for (size_t i = 0; i < result.capacity; i++)
        {
            if (i >= size && i >= number.size && extra == 0)
                break;

            char left = (i < size) ? data[i] : 0;
            char right = (i < number.size) ? 
                        (number.data[i] - extra / 10) : 0;

            extra = (right >= left) ? 0 : 10;
            result.data[result.size++] =  (right - left + extra) % 10;
        }
    }

    void l_add(const BigInt& number, BigInt& result) const 
    {
        char extra = 0;

        for (size_t i = 0; i < result.capacity; i++) 
        {
            if (i >= size && i >= number.size && extra == 0)
                break;

            char left = (i < size) ? (data[i] - extra / 10) : 0;
            char right = (i < number.size) ? number.data[i] : 0;

            extra = (left >= right) ? 0 : 10;
            result.data[result.size++] =  (left - right + extra) % 10;
        }
    }


public:
    
    BigInt() : data(new char[capacity]), size(1)
    {
        std::memset(data, 0, capacity * sizeof(char));
    }


    BigInt(int64_t number) : data(new char[capacity]), sign(number < 0)
    {
        if (number == 0) 
            size = 1;
        
        number *= sign ? -1 : 1;

        std::memset(data, 0, capacity * sizeof(char));

        while (number)
        {
            data[size++] = number % 10;
            number = number / 10;

            if (size == capacity) 
            {
                char* temp = data;
                data = new char[2 * capacity];
                std::memcpy(data, temp, capacity * sizeof(char));
                capacity *= 2;

                delete[] temp;
            }
        }
    }

    BigInt(const BigInt& copied): size(copied.size), capacity(copied.capacity), sign(copied.sign)
    {
        if (data != nullptr)
            delete[] data;
        data = new char[capacity];
        
        std::memcpy(data, copied.data, capacity * sizeof(char));
    }

    ~BigInt() 
    {
        delete[] data;
    }

    BigInt& operator=(const BigInt& copied)
    {
        if (data != nullptr)
            delete[] data;

        size = copied.size;
        capacity = copied.capacity;
        sign = copied.sign;
        data = new char[capacity];
        std::memcpy(data, copied.data, capacity * sizeof(char));

        return *this;
    }

    bool operator==(const BigInt& number) const 
    {
        return size == number.size && 
               sign == number.sign && 
               !std::memcmp(data, number.data, size * sizeof(char));
    }

    
    BigInt operator+(const BigInt& number) const
    {
        BigInt res;
        delete[] res.data;

        res.capacity = std::max(capacity, number.capacity) + 1;
        res.size = 0;
        res.data = new char[res.capacity];
        std::memset(res.data, 0, res.capacity * sizeof(char));

        if (sign == number.sign) 
        {
            res.sign = sign;
            char extra = 0;
            for (size_t i = 0; i < res.capacity; i++)
            {
                if (i >= size && i >= number.size && extra == 0)
                    break;

                char left = (i < size) ? data[i] : 0;
                char right = (i < number.size) ? number.data[i] : 0;
                char sum = (left + right + extra);

                res.data[res.size++] = sum % 10;
                extra = sum / 10;
            }

        } 
        
        else if (sign) 
        {
            if (-*this < number) 
            {
                res.sign = false;
                r_add(number, res);
            } 
            else
            {
                res.sign = true;
                l_add(number, res);
            }
        } 
        
        else 
        {
            if (*this <= -number) 
            {
                res.sign = true;
                r_add(number, res);
            } 
            else 
            {
                res.sign = false;
                l_add(number, res);
            }
        }

        
        return res.UpdateSize();
    }
    
    
    
    BigInt operator-() const 
    {
        BigInt number(*this);
        if (number != 0)
            number.sign = !sign;

        return number;
    }
    
    
    bool operator!=(const BigInt& number) const 
    {
        return !(*this == number);
    }

    bool operator<(const BigInt& number) const
    {
        if (sign == number.sign) 
        {
            if (size == number.size) 
            {

                int diff = 0;
                for (int i = size - 1; (!diff && i >= 0); i--)
                    diff = data[i] - number.data[i];

                return (diff != 0) && (diff < 0 ? !sign : sign);

            } 
            else 
            {
                return (size < number.size) ? !sign : sign;
            }
            
        } 
        else
        {
            return sign;
        }
    }

    bool operator>(const BigInt& number) const
    {
        return !(*this <= number);
    }
    
    
    bool operator<=(const BigInt& number) const
    {
        return (*this < number) || (*this == number);
    }

    bool operator>=(const BigInt& number) const
    {
        return !(*this < number);
    }



    BigInt operator-(const BigInt& number) const 
    {
        return *this + (-number);
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& number)
    {
        if (number.sign)
            out << "-";

        for (size_t i = 0; i < number.size; i++)
            out << (int)number.data[number.size - i - 1];

        return out;
    }
};