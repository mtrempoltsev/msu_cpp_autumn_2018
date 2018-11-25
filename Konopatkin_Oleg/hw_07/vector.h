#include <iostream>
#include <cstdlib>
#include <limits>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;

    pointer allocate(size_type cnt)
    {
        return static_cast<pointer>(::operator new(cnt * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type cnt)
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
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;    
    using difference_type = std::ptrdiff_t;

private:
    pointer iter;
    bool is_forward;

public:
    explicit Iterator(pointer p, bool forward) : iter(p), is_forward(forward) {}

    Iterator& operator++() {
        if (is_forward) {
            ++iter;
        } else {
            --iter;
        }

        return *this;
    }

    Iterator operator++(int) {
        Iterator cur = *this;
        ++(*this);
        return cur;
    }

    Iterator& operator--() {
        if (is_forward) {
            --iter;
        } else {
            ++iter;
        }
        return *this;
    }

    Iterator operator--(int) {
        Iterator cur = *this;
        --(*this);
        return cur;
    }

    bool operator==(Iterator other) const {
        return iter == other.iter;
    }

    bool operator!=(Iterator other) const {
        return iter != other.iter;
    }

    reference operator*() const {
        return *iter;
    }
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = Iterator<T>;

private:
    Alloc alloc_;
    pointer data_;
    size_type len_;  // длина вектора
    size_type size_; // размер буфера
    static const size_type BASE_SIZE = 256;

public:

    Vector () : len_(0), alloc_(), size_(BASE_SIZE), data_(alloc_.allocate(size_)) {}

    Vector (size_type size, value_type&& def) : len_(size), alloc_(), size_(size), data_(alloc_.allocate(size_)) {
        for (size_type i = 0; i < len_; ++i) {
            data_[i] = def;
        }
    }

    ~Vector() {
        for (size_type i = 0; i < len_; ++i) {
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, size_);
    }

    reference operator[] (size_type ind) {
        return data_[ind];
    }

    const reference operator[] (size_type ind) const {
        return data_[ind];
    }

    void reserve(size_type newSize) {
        if (size_ < newSize)
        {
            auto newData = alloc_.allocate(newSize);
            std::move(data_, data_ + size_, newData);
            alloc_.deallocate(data_, size_);
            data_ = newData;
            size_ = newSize;
        }
    }

    void resize(size_type newSize, value_type&& def = value_type()) {
        if (len_ < newSize)
        {
            this->reserve(newSize * 2);
            for (size_type i = len_; i < newSize; ++i) {
                data_[i] = def;
            }
            len_ = newSize;
        } else if (len_ > newSize) {
            for (size_type i = newSize; i < len_; ++i) {
                alloc_.destroy(data_ + i);
            }
            len_ = newSize;
        }
    }

    void push_back(value_type&& value) {
        if (len_ == size_) {
            this->reserve(size_ * 2);
        }
        data_[len_] = value;
        ++len_;
    }

    void pop_back() {
        alloc_.destroy(data_ + len_ - 1);
        --len_;
    }

    bool empty() const {
        return len_ == 0;
    }

    size_type size() const {
        return len_;
    }

    size_type capacity() const {
        return size_;
    }

    void clear() {
        for (size_type i = 0; i < len_; ++i) {
            alloc_.destroy(data_ + i);
        }
        len_ = 0;
    }

    Iterator<value_type> begin() {
        return Iterator<value_type>(data_, true);
    }

    Iterator<value_type> end() {
        return Iterator<value_type>(data_ + len_, true);
    }

    Iterator<value_type> rbegin() {
        return Iterator<value_type>(data_ + len_ - 1, false);
    }

    Iterator<value_type> rend() {
        return Iterator<value_type>(data_ - 1, false);
    }
};

/* 
Vector<int>: 74004 us
std::vector<int>: 89005 us
std::deque<int>: 207011 us
std::list<int>: 5817332 us
*/

/* На сервере:
Vector<int>: 63381 us
std::vector<int>: 90338 us
std::deque<int>: 188175 us
std::list<int>: 3545155 us
*/
