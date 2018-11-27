#pragma once
#include <exception>
#include <iterator>
#include <cstring>


template <class T>
class Allocator
{
public:
    T* allocate(std::size_t n) 
    {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template <class U, class... Args>
    void construct(U* p, Args&&... args)
    {
        new((void *)p) U(std::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U* p)
    {
        p->~U();
    }

    void deallocate(T* p)
    {
        ::operator delete(p);
    }
};

template <class T>
class Iterator: 
    public std::iterator<std::forward_iterator_tag, T>
{
public:
    using iterator = Iterator<T>;

    Iterator() : pos_(nullptr) {}
    Iterator(T* v) : pos_(v) {}
    ~Iterator() {}

    iterator operator++(int)
    {
        return pos_++;
    }

    iterator& operator++()
    {
        ++pos_; 
        return *this;
    }

    iterator operator--(int)
    {
        return pos_--;
    }

    iterator& operator--()
    {
        --pos_; 
        return *this;
    }

    T& operator*()
    { 
        return *pos_;
    }

    bool operator==(const iterator& rhs) const
    {
        return pos_ == rhs.pos_;
    }

    bool operator!=(const iterator& rhs) const
    {
        return pos_ != rhs.pos_;
    }

private:
    T* pos_;
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using iterator = Iterator<T>;

    Vector() : size_(0), capacity_(8)
    {
        data_ = alloc_.allocate(capacity_);
    }

    ~Vector()
    {
        for (auto p = data_; p < data_ + size_; p++)
            alloc_.destroy(p);

        alloc_.deallocate(data_);
    }

    T& operator[](std::size_t ind)
    {
        if (ind >= size_)
            throw std::out_of_range("");

        return data_[ind];
    }

    void push_back(T&& value)
    {
        if (size_ == capacity_)
            reserve(2 * capacity_ + 1);

        alloc_.construct(data_ + size_, std::move(value));
        size_++;
    }

    void push_back(const T& value)
    {
        if (size_ == capacity_)
            reserve(2 * capacity_ + 1);

        alloc_.construct(data_ + size_, value);
        size_++;
    }

    void pop_back()
    {
        if (size_ > 0)
        {
            size_--;
            alloc_.destroy(data_ + size_);  
        }   
    }

    void reserve(std::size_t newSize)
    {
        if (capacity_ < newSize)
        {
            T* newData = alloc_.allocate(newSize);;
            std::memmove(newData, data_, capacity_ * sizeof(T));
            std::swap(data_, newData);
            capacity_ = newSize;
            alloc_.deallocate(newData);
        }
    }

    void resize(std::size_t count)
    {
        if (count > capacity_)
            reserve(count);

        for (; size_ > count; size_--)
            alloc_.destroy(data_ + size_ - 1);

        for (; size_ < count; size_++)
            alloc_.construct(data_ + size_);
    }

    bool empty() const
    {
        return size_ == 0;
    }

    std::size_t size() const
    {
        return size_;
    }

    std::size_t capacity() const
    {
        return capacity_;
    }

    void clear()
    {
        for (; size_ > 0; size_--)
            alloc_.destroy(data_ + size_ - 1);
    }

    auto begin()
    {
        return iterator(data_);
    }

    auto end()
    {
        return iterator(data_ + size_);
    }

    auto rbegin()
    {
        return std::reverse_iterator<iterator>(data_ + size_);
    }

    auto rend()
    {
        return std::reverse_iterator<iterator>(data_);
    }

private:
    T* data_;
    Alloc alloc_;
    std::size_t size_;
    std::size_t capacity_;
};