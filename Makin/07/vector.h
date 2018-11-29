#pragma once

#include <iostream>
#include <iterator>


template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T *;
    using size_type = std::size_t;

    pointer allocate(size_type count)
    {
        return static_cast<pointer>(::operator new(count * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type count)
    {
        ::operator delete(ptr);
    }

    template <class... Args>
    void construct(T *ptr, Args&&... args)
    {
        ::new (ptr) T(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr)
    {
        ptr->~T();
    }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }
};


template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
    T* ptr_;
public:

    using reference = T&;

    explicit Iterator(T* ptr)
        : ptr_(ptr)
    {
    }

    bool operator==(const Iterator<T>& other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const
    {
        return !(*this == other);
    }

    reference operator*() const
    {
        return *ptr_;
    }

    Iterator& operator++()
    {
        ++ptr_;
        return *this;
    }
     Iterator& operator--()
    {
        --ptr_;
        return *this;
    } 
  
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using const_reference = const T&;
    using iterator = Iterator<T>;

private:
    
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type max_size_;   
    
public:
    
    explicit Vector(size_type size) : size_(size), max_size_(size * 2)
    {
        data_ = alloc_.allocate(max_size_);
        for (size_type i = 0; i < size; ++i)
        {
            alloc_.construct(data_[i]);
        }
    }
    
    explicit Vector() : size_(0), max_size_(8)
    {
        data_ = alloc_.allocate(max_size_);
    }
    
    ~Vector()
    {
        clear();
        alloc_.deallocate(data_, max_size_);
    }
    

    bool empty() const noexcept
    {
        return size_ == 0;
    }
    
    void clear()
    {
        resize(0);
    }

    size_type size() const noexcept
    {
        return size_;
    }
    

    void push_back(const_reference value)
    {
        if (size_ >= max_size_)
        {
            reserve((max_size_ > 0) ? (2 * max_size_) : 8);
        }
        alloc_.construct(data_ + size_++, value);
    }   
    
    void push_back(value_type&& value)
    {
        if (size_ >= max_size_)
        {
            reserve((max_size_ > 0) ? (2 * max_size_) : 8);
        }
        alloc_.construct(data_ + size_++, std::move(value));
    }
    

    reference operator [](size_type pos)
    {
        if (pos < size_)
            return data_[pos];
        throw std::out_of_range("Error");
    }
    
    const_reference operator [](size_type pos) const
    {
        if (pos < size_)
            return data_[pos];
        throw std::out_of_range("Error");
    }


    void pop_back()
    {
        if (size_) 
        {
            alloc_.destroy(data_ + size_);
            --size_;
            return;
        }
        
        throw std::out_of_range("Error");
    }

    void reserve(const size_type count) 
    {
        if (max_size_ < count) 
        {
            pointer new_data = alloc_.allocate(count);
            for (size_type i = 0; i < size_; i++) 
            {
                alloc_.construct(new_data + i, 
                    std::move(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }

            alloc_.deallocate(data_, max_size_);
            max_size_ = count;
            data_ = new_data;
        }   
    }

    size_type capacity() const noexcept 
    {
        return max_size_;
    }  



    void resize(size_type count)
    {
        if (size_ > count)
            for (size_type i = count; i < size_; ++i)
                alloc_.destroy(data_ + i);
        else
        {
            if (max_size_ < count)
                reserve(count);

            for (size_type i = size_; i < count; ++i)
            {
                alloc_.construct(data_ + i);
            }
        }
        size_ = count;
    }


    iterator begin() const noexcept
    {
        return iterator(data_);
    }

    iterator end() const noexcept
    {
        return iterator(data_ + size_);
    }
    
    reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend() const noexcept
    {
        return reverse_iterator(begin());
    } 
};
