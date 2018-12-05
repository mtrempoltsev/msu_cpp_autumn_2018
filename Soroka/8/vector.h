#pragma once
#include <exception>
#include <iterator>
#include <cstring>

template <class T>
class Allocator
{
public:
    T* allocate(size_t n)
    {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* tp)
    {
        ::operator delete(tp);
    }

    template <class... Args>
    void construct(T* tp, Args&&... args)
    {
        new(tp) T(std::forward<Args>(args)...);
    }

    void destroy(T* tp)
    {
        tp->~T();
    }

};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
    T* _position;
public:
    Iterator(){}
    Iterator(T* position)
    {
        _position = position;
    }
    ~Iterator() {}
    T& operator*()
    {
        return *_position;
    }
    T operator*() const
    {
        return *_position;
    }
    bool operator==(const Iterator<T>& data) const
    {
        return _position == data._position;
    }
    bool operator!=(const Iterator<T>& data) const
    {
        return !(_position == data._position);
    }
    Iterator<T> operator++(int)
    {
        return _position++;
    }
    Iterator<T> operator--(int)
    {
        return _position--;
    }
    Iterator<T>& operator++()
    {
        ++_position;
        return *this;
    }
    Iterator<T>& operator--()
    {
        --_position;
        return *this;
    }
    Iterator<T> operator+(const size_t val, const Iterator<T>& it)
    {
        return it + val;
    }
    Iterator<T> operator+(const size_t val) const
    {
        return _position + val;
    }
    Iterator<T>& operator+=(const size_t& val)
    {
        for (size_t it = 0; it < val; it++)
        {
            ++_position;
        }
        return *this;
    }
    Iterator<T> operator-(const size_t val) const
    {
       return _position - val;
    }
    Iterator<T>& operator-=(const size_t& val)
    {
        for (size_t it = 0; it < val; it++)
        {
            --_position;
        }
        return *this;
    }
    bool operator<(const Iterator<T>& it) const
    {
        return distance(it - _position) > 0;
    }
    bool operator<=(const Iterator<T>& it) const
    {
        return distance(it - _position) >= 0;
    }
    bool operator>(const Iterator<T>& it) const
    {
        return distance(it - _position) < 0;
    }
    bool operator>=(const Iterator<T>& it) const
    {
        return distance(it - _position) <= 0;
    }
    Iterator<T> operator[](const size_t val) const
    {
        return _position + val;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
    Alloc alloc_;
    T* data;
    size_t length;
    size_t capacity_length;
public:
    using iterator = Iterator<T>;

    Vector()
    {
        length = 0;
        capacity_length = 4;
        data = alloc_.allocate(capacity_length);
    }

    ~Vector()
    {
        auto d = data;
        while((d++) < data + length)
            alloc_.destroy(d);
        alloc_.deallocate(data);
    }

    void clear()
    {
        for (size_t it = 0; it < length; it++) {
                alloc_.destroy(data + it);
        }
        length = 0;
        capacity_length = 0;
    }

    auto begin()
    {
        return iterator(data);
    }

    auto end()
    {
        return iterator(data + length);
    }

    auto rbegin()
    {
        return std::reverse_iterator<iterator>(data + length);
    }

    auto rend()
    {
        return std::reverse_iterator<iterator>(data);
    }

    bool empty() const
    {
        return length == 0;
    }

    size_t size() const
    {
        return length;
    }

    size_t capacity() const
    {
        return capacity_length;
    }

    T& operator[](size_t index)
    {
        if (index >= length)
            throw std::out_of_range("index out of range");
        return data[index];
    }

    void push_back(T&& value)
    {
        if (length == capacity_length)
            reserve(capacity_length << 1);
        alloc_.construct(data + length, std::move(value));
        length++;
    }

    void push_back(const T& value)
    {
        if (length == capacity_length)
            reserve(capacity_length << 1);
        alloc_.construct(data + length, value);
        length++;
    }

    void pop_back()
    {
        if (length > 0)
        {
            length--;
            alloc_.destroy(data + length);
        }
    }

    void resize(size_t re_size)
    {
        if (length > re_size) {
            for (size_t it = re_size; it < length; it++) {
                alloc_.destroy(data + it);
            }
        } else {
            if (capacity_length < re_size)
                reserve(re_size << 1);
            for (size_t it = length; it < re_size; it++)
                alloc_.construct(data + it);
        }
        length = re_size;
    }

    void reserve(size_t reserveSize)
    {
        if (capacity_length < reserveSize) {
            for (size_t it = 0; it < length; it++) {
                alloc_.destroy(data + it);
            }
            T* temp = alloc_.allocate(reserveSize);
            for (size_t it = 0; it < length; it++) {
                alloc_.construct(temp + it, std::forward<T>(*(data + it)));
            }
            alloc_.deallocate(data);
            data = temp;
            capacity_length = reserveSize;
        }
    }
};
