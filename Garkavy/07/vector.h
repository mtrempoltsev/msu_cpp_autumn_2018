#pragma once

#include <iterator>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    template<class... ArgsT>
    void construct(pointer ptr, ArgsT&&... args)
    {
        ::new (ptr) value_type(std::forward<ArgsT>(args)...);
    }

    pointer allocate(size_type size)
    {
        return static_cast<pointer>(
            ::operator new(size * sizeof(value_type))
        );
    }

    void deallocate(pointer ptr, size_type size)
    {
        ::operator delete(ptr, size * sizeof(value_type));
    }

    void destroy(pointer ptr)
    {
        ptr->~value_type();
    }
};

template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using ref = T&;
    using iterator = Iterator<value_type>;
    using diff_type = std::ptrdiff_t;

private:
    pointer ptr_;

public:
    explicit Iterator(pointer ptr) noexcept
        : ptr_(ptr)
    {
    }

    ref operator*() const noexcept
    {
        return *ptr_;
    }

    iterator& operator[](int idx)
    {
        return ptr_[idx];
    }

    bool operator==(const iterator& other) const noexcept
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const iterator& other) const noexcept
    {
        return ptr_ != other.ptr_;
    }

    bool operator<(const iterator& other) const noexcept
    {
        return ptr_ < other.ptr_;
    }

    bool operator>(const iterator& other) const noexcept
    {
        return ptr_ > other.ptr_;
    }

    bool operator<=(const iterator& other) const noexcept
    {
        return ptr_ <= other.ptr_;
    }

    bool operator>=(const iterator& other) const noexcept
    {
        return ptr_ >= other.ptr_;
    }

    iterator& operator++() noexcept
    {
        ++ptr_;
        return *this;
    }

    iterator& operator--() noexcept
    {
        --ptr_;
        return *this;
    }

    iterator operator++(int) noexcept
    {
        auto tmp = this;
        ++ptr_;
        return *tmp;
    }

    iterator operator--(int) noexcept
    {
        auto tmp = this;
        --ptr_;
        return *tmp;
    }

    diff_type operator-(const iterator& other) noexcept
    {
        diff_type diff = ptr_ - other.ptr_;
        return diff;
    }

    iterator operator+(diff_type diff) noexcept
    {
        auto tmp = this;
        ptr_ += diff;
        return *tmp;
    }

    iterator operator-(diff_type diff) noexcept
    {
        auto tmp = this;
        ptr_ -= diff;
        return *tmp;
    }

    iterator& operator+=(diff_type diff) noexcept
    {
        ptr_ += diff;
        return *this;
    }

    iterator& operator-=(diff_type diff) noexcept
    {
        ptr_ -= diff;
        return *this;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using value_type = T;
    using pointer = T*;
    using ref = T&;
    using iterator = Iterator<value_type>;
    using size_type = size_t;

private:
    Alloc alloc_;
    size_type capacity_;
    size_type size_;
    pointer data_;
    static constexpr size_type base_size_{1};

public:
    Vector(size_type size = 0)
        : alloc_(),
        capacity_(size > 0 ? size : base_size_),
        data_(alloc_.allocate(capacity_)),
        size_(size)
    {
        for(pointer ptr = data_; ptr != data_ + size_; ++ptr) {
            alloc_.construct(ptr);
        }
    }

    ~Vector()
    {
        for(pointer ptr = data_; ptr != data_ + size_; ++ptr) {
            alloc_.destroy(ptr);
        }
        alloc_.deallocate(data_, capacity_);
    }

    size_type size() const noexcept
    {
        return size_;
    }

    size_type capacity() const noexcept
    {
        return capacity_;
    }

    bool empty() const noexcept
    {
        return (size_ == 0);
    }

    const ref operator[](size_type idx) const
    {
        if(idx >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[idx];
    }

    ref operator[](size_type idx)
    {
        if(idx >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[idx];
    }

    void clear()
    {
        for(pointer ptr = data_; ptr != data_ + size_; ++ptr) {
            alloc_.destroy(ptr);
        }
        size_ = 0;
    }

    void reserve(size_type new_capacity)
    {
        if(new_capacity <= capacity_) {
            return;
        }
        pointer tmp = alloc_.allocate(new_capacity);
        for(size_type i = 0; i < size_; ++i) {
            alloc_.construct(tmp + i, std::move(*(data_ + i)));
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, capacity_);
        data_ = tmp;
        capacity_ = new_capacity;
    }

    void resize(size_type new_size)
    {
        if(new_size < size_) {
            for (pointer ptr = data_ + new_size; ptr != data_ + size_; ++ptr) {
                alloc_.destroy(ptr);
            }
        } else {
            if(new_size > capacity_) {
                reserve(new_size);
            }
            for(pointer ptr = data_ + size_; ptr != data_ + new_size; ++ptr) {
                alloc_.construct(ptr);
            }
        }
        size_ = new_size;
    }

    iterator begin()
    {
        return iterator(data_);
    }

    iterator end()
    {
        return iterator(data_ + size_);
    }

    std::reverse_iterator<iterator> rbegin()
    {
        return std::reverse_iterator<iterator>(end());
    }

    std::reverse_iterator<iterator> rend()
    {
        return std::reverse_iterator<iterator>(begin());
    }

    void push_back(const ref val)
    {
        if(size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        alloc_.construct(data_ + size_, val);
        ++size_;
    }

    void push_back(value_type&& val)
    {
        if(size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        alloc_.construct(data_ + size_, std::move(val));
        ++size_;
    }

    void pop_back()
    {
        if(size_ == 0) {
            throw std::out_of_range("pop back from empty vector");
        }
        alloc_.destroy(data_ + size_ - 1);
        --size_;
    }
};
