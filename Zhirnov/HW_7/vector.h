#pragma once

#include <stdexcept>
#include <limits>
#include <iterator>
#include <cstring>
#include <algorithm>


template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    
    size_type max_size() const noexcept
    {
    // Maximum size possible to allocate
        return std::numeric_limits<size_type>::max();
    }

    pointer allocate(size_type n)
    {
    // Allocate block of storage 
        if (n > max_size())
            throw std::invalid_argument("Error, size too big!");
        if (n == 0)
            return nullptr;

        return static_cast<pointer>(malloc(n * sizeof(value_type)));
    } 

    void deallocate (pointer p)
    {
    // Release block of storage
        free(p);
    }

    void construct (pointer p)
    { 
    // Constructs an element object on the location pointed by p.
    // Inizialize with default value
        new (p) value_type();
    }

    void construct(pointer p, const_reference val)
    {
    // Constructs an element object on the location pointed by p.
    // Inizialize with val
        new (p) value_type(val);
    }

    void destroy(pointer p)
    {
    // Destroys in-place the object pointed by p.
        p->~value_type();
    }

};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = std::size_t;
    using reference = T&;
    
    Iterator() : _ptr(nullptr) {}

    explicit Iterator(pointer ptr):_ptr(ptr) {};

    Iterator(const Iterator<value_type> & other):_ptr(other._ptr) {};

    ~Iterator() {}

    Iterator& operator[](const size_t index) const
    {
        return *(_ptr + index);
    }

    Iterator& operator +=(const size_t index)
    {
        _ptr += index;
        return *this;
    }
    
    Iterator operator +(const size_t index)
    {
        auto temp = *this;
        return temp += index;
    }

    friend Iterator operator+(const size_t ind, const Iterator<value_type> & other)
    {
        return other + index;
    }
    
    Iterator& operator -=(const size_t index)
    {
        return *this += -index;
    }
    
    Iterator operator -(const Iterator<value_type> & other)
    {
        return _ptr - other._ptr;
    }

    bool operator <(const Iterator<value_type> & other)
    {
        return (operator- (other) > 0);
    }
    
    bool operator >(const Iterator<value_type> & other)
    {
        return other < *this;
    }
    
    bool operator >=(const Iterator<value_type> & other)
    {
        return !(*this < other);
    }
    
    bool operator <=(const Iterator<value_type> & other)
    {
        return !(*this > other);
    }

    Iterator& operator++()
    {
    // Increment iterator by one and return it

        _ptr++;
        return *this;
    }
    
    Iterator& operator++(int)
    {
    // Increment iterator by one and return non incremented
            
        Iterator tmp(*this);
            operator++();
            return tmp;
        }

    Iterator& operator--()
    {
    // Decrement iterator by one and return it
    
        _ptr--;
        return *this;
    }

    Iterator& operator--(int)
    {
    // Decrement iterator by one and return non decremented

            Iterator tmp(*this);
            operator--();
            return tmp;
        }

    bool operator==(const Iterator<value_type> & other) const
    {
    // Compares the underlying iterators
    
        return _ptr == other._ptr;
    }
    
    bool operator!=(const Iterator<value_type> & other) const
    {
    // Compares the underlying iterators
    
        return _ptr != other._ptr;
    }

    reference operator*() const
    {
        return *_ptr;   
    }
    
    Iterator operator=(Iterator<value_type> other)
    {
        _ptr = other._ptr;
        return *this;
    }

private:
    T* _ptr;
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using allocator_type = Alloc;
    using reference = value_type&;  
    using const_reference   = const value_type&;    
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = const std::reverse_iterator<iterator>;

private:
    pointer _data;
    Alloc _alloc;
    size_type _size;
    size_type _capacity;

public:

    Vector(): _capacity(0), _size(0), _data(nullptr){};             
        
    Vector(size_type count, const_reference val = value_type()): _capacity(count), _size(count), _data(nullptr)
    {
        _data = _alloc.allocate(count);
        for (int i = 0; i < count; i++)
            _alloc.construct(_data + i, val);
    }             

    ~Vector()
    {
    // Destructor
        for (int i = 0; i < _size; i++)
            _alloc.destroy(_data + i);

        _alloc.deallocate(_data);
    }

    size_type size() const
    {
        return _size;
    }

    iterator begin()
    {
        return iterator(_data);
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    
    const_reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(end());
    }

    iterator end()
    {
        return iterator(_data + _size);
    }
    
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return reverse_iterator(begin());
    }

    bool empty() const{
        return _size == 0;
    }

    size_type capacity() const
    {
        return _capacity;
    }

    void reserve(size_type size)
        {
            if (size > _capacity)
            {
            pointer tmp_ptr = _alloc.allocate(size);
            for (int i = 0; i < _size; i++)
            {
                _alloc.construct(tmp_ptr + i, *(_data + i));
                _alloc.destroy(_data + i);
            }
            
            _alloc.deallocate(_data);
            _data = tmp_ptr;

            _capacity = size;
                };
        return;
    }

    Vector& operator=(const Vector & other)
    {
        for (int i = 0; i < _size; i++)
            _alloc.destroy(_data + i);

        _alloc.deallocate(_data);

        _data = _alloc.allocate(other._capacity);
        for (int i = 0; i < other._size; i++)
            _alloc.construct(_data + i, *(other._data + i));
        
        return *this;
    }

    reference operator[](size_type pos)
    {
        return *(_data + pos);
    }

    const_reference operator[] (size_type pos) const
    {
        return *(_data + pos);
    }
    
    reference at(size_type pos)
    {
        if ((pos < 0) || (pos >= _size))    
            throw std::invalid_argument("Error, position should be in [0, size].");
        return *(_data + pos);
    }

    const_reference at(size_type pos) const
    {
        if ((pos < 0) || (pos >= _size))    
            throw std::invalid_argument("Error, position should be in [0, size].");
        return *(_data + pos);
    }

    reference front()
    {
        return *(_data);
    }

    const_reference front() const
    {
        return *(_data);
    }

    reference back()
    {
        return *(_data + size - 1);
    }

    const_reference back() const
    {
        return *(_data + size - 1);
    }

    void clear()
    {
        for (int i = 0; i < _size; i++)
            _alloc.destroy(_data + i);
        _size = 0;
    };     

    void resize(size_type count, const_reference value = value_type())
    {
        if (count <= _size){
            for (int i = count; i < _size; i++)
                _alloc.destroy(_data + i);
        };
        if (count > _capacity)
            reserve(count);
        if (count > _size)
            for (int i = _size; i < count; i++)
                _alloc.construct(_data + i, value);
        _size = count;
    }

    void push_back(const_reference value)
    {
        if (_size == _capacity)
            if (_size == 0)
                reserve(1);
            else
                reserve(_capacity * 2);

        _alloc.construct(_data + _size, value);
        _size++;
    }

    void pop_back()
    {
        if (_size == 0)
            throw std::invalid_argument("Container should be non empty.");
        _alloc.destroy(_data + _size - 1);
        _size--;
    }
};
