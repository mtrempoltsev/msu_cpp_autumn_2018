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
    void create(T* tp, Args&&... args)
    {
        new(tp) T(std::forward<Args>(args)...);
    }


    void del(T* tp)
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
            alloc_.del(d);
        alloc_.deallocate(data);
    }

    void clear()
    {
        for (size_t it = 0; it < length; it++) {
                alloc_.del(data + it);
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
            reserve(capacity_length * 2);
        alloc_.create(data + length, value);
        length++;
    }

    void push_back(const T& value)
    {
        if (length == capacity_length)
            reserve(capacity_length * 2);
        alloc_.create(data + length, value);
        length++;
    }

    void pop_back()
    {
        if (length > 0)
        {
            length--;
            alloc_.del(data + length);
        }
    }

    void resize(size_t re_size)
    {
        if (length > re_size) {
            for (size_t it = re_size; it < length; it++) {
                alloc_.del(data + it);
            }
        } else {
            if (capacity_length < re_size)
                reserve(re_size << 1);
            for (size_t it = length; it < re_size; it++)
                alloc_.create(data + it);
        }
        length = re_size;
    }

    void reserve(size_t reserveSize)
    {
        if (capacity_length < reserveSize)
        {
            T* temp = alloc_.allocate(reserveSize);
            memmove(temp, data, capacity_length * sizeof(T));
            capacity_length = reserveSize;
            std::swap(data, temp);
            alloc_.deallocate(temp);
        }
    }
};
