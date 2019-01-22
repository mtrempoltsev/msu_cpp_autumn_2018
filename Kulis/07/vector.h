#pragma once
#include <cstddef> // size_t
#include <iterator>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_t n)
    {
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type size)
    {
        ::operator delete(ptr, size * sizeof(value_type));
    }

    template <typename ... Args>
    void construct(pointer ptr, Args && ... args)
    {
        new (ptr) value_type(std::forward<Args>(args) ...);
    }

    void destroy(pointer ptr)
    {
        ptr->~value_type();
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using iterator = Iterator<T>;

    Iterator(pointer ptr) noexcept
        : ptr_(ptr)
    {}

    reference operator * () const noexcept
    {
        return *ptr_;
    }

    bool operator == (const iterator & other) const noexcept
    {
        return ptr_ == other.ptr_;
    }

    bool operator != (const iterator & other) const noexcept
    {
        return ptr_ != other.ptr_;
    }

    bool operator < (const iterator & other) const noexcept
    {
        return ptr_ < other.ptr_;
    }

    bool operator > (const iterator & other) const noexcept
    {
        return ptr_ > other.ptr_;
    }

    bool operator <= (const iterator & other) const noexcept
    {
        return ptr_ <= other.ptr_;
    }

    bool operator >= (const iterator & other) const noexcept
    {
        return ptr_ >= other.ptr_;
    }

    iterator operator + (int n) const noexcept
    {
        return iterator(ptr_ + n);
    }

    iterator & operator += (int n) noexcept
    {
        ptr_ += n;
        return *this;
    }

    iterator operator - (int n) const noexcept
    {
        return iterator(ptr_ - n);
    }

    iterator & operator -= (int n) noexcept
    {
        ptr_ -= n;
        return *this;
    }

    reference operator [] (int n) const noexcept
    {
        return ptr_[n];
    }

    iterator & operator ++ () noexcept
    {
        ++ptr_;
        return *this;
    }

    iterator & operator -- () noexcept
    {
        --ptr_;
        return *this;
    }

    iterator operator ++ (int) noexcept
    {
        iterator tmp = *this;
        ++ptr_;
        return tmp;
    }

    iterator operator -- (int) noexcept
    {
        iterator tmp = *this;
        --ptr_;
        return tmp;
    }

    friend iterator& operator + (int n, const iterator & it) noexcept
    {
        return n + it.ptr_;
    }

    friend iterator& operator - (int n, const iterator & it) noexcept
    {
        return n - it.ptr_;
    }
private:
    pointer ptr_;
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using iterator = Iterator<T>;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using size_type = size_t;
    using reverse_iterator = std::reverse_iterator<iterator>;

    Vector()
        : size_(0)
        , capacity_(8)
    {
        data_ = alloc_.allocate(capacity_);
    }

    ~Vector()
    {
        for (pointer ptr = data_; ptr != data_ + size_; ++ptr) {
            alloc_.destroy(ptr);
        }
        alloc_.deallocate(data_, capacity_);
    }

    reference operator[] (size_type n)
    {
        if (n >= size_) {
            throw std::out_of_range("out of range");
        }
        return data_[n];
    }

    const reference operator[] (size_type n) const
    {
        if (n >= size_) {
            throw std::out_of_range("out of range");
        }
        return data_[n];
    }

    void push_back(const T & value)
    {
        if (size_ >= capacity_) {
            reserve(capacity_ * 2);
        }

        alloc_.construct(data_ + size_, value);
        ++size_;
    }

    void push_back(T && value)
    {
        if (size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        alloc_.construct(data_ + size_, std::move(value));
        ++size_;
    }

    void pop_back()
    {
        if (size_ > 0) {
            --size_;
            alloc_.destroy(data_ + size_);
        } else {
            throw std::out_of_range("empty vector");
        }
    }

    void clear()
    {
        for (pointer ptr = data_; ptr != data_ + size_; ++ptr) {
            alloc_.destroy(ptr);
        }
        size_ = 0;
    }

    void reserve(size_type new_size)
    {
        if (new_size > capacity_) {
            pointer tmp = alloc_.allocate(new_size);

            for (size_type i = 0; i < size_; ++i) {
                alloc_.construct(tmp + i, std::move(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }

            alloc_.deallocate(data_, capacity_);
            data_ = tmp;
            capacity_ = new_size;
        }
    }

    void resize(size_type new_size)
    {
        if (new_size < size_) {
            for (pointer ptr = data_ + new_size; ptr < data_ + size_; ++ptr) {
                alloc_.destroy(ptr);
            }
        } else if (new_size > size_) {
            if (new_size > capacity_) {
                reserve(new_size);
                capacity_ = new_size;
            }
            for (pointer ptr = data_ + size_; ptr < data_ + new_size; ++ptr) {
                alloc_.construct(ptr);
            }
        }
        size_ = new_size;
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
        return size_ == 0;
    }

private:
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type capacity_;
};
