#pragma once
#include <exception>
#include <iterator>
#include <new>
#include <limits>

template<class T>
class Allocator {
public:
    T* address(T& x) const noexcept {
        return &x;
    }

    const T* adress(const T& x) const noexcept {
        return &x;
    }

    T* allocate(size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p) {
        ::operator delete(static_cast<void*>(p));
    }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    template<typename... Args>
    void construct(T* p, Args&&... args) {
        ::new ((void*) p) T(std::forward<Args>(args)...);
    }

    void destroy(T* p) {
        p->~T();
    }
};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
    T* p_;
public:
    Iterator(){}

    Iterator(T* position) {
        p_ = position;
    }

    ~Iterator() {}

    bool operator==(const Iterator<T>& data) const {
        return p_ == data.p_;
    }

    bool operator!=(const Iterator<T>& data) const {
        return !(p_ == data.p_);
    }

    T& operator*() {
        return *p_;
    }

    T operator*() const {
        return *p_;
    }

    Iterator<T>& operator++() {
        ++p_;
        return *this;
    }

    Iterator<T> operator++(int) {
        return p_++;
    }

    Iterator<T>& operator--() {
        --p_;
        return *this;
    }

    Iterator<T> operator--(int) {
        return p_--;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
    size_t size_;
    size_t capacity_;
    T* data_;
    Alloc alloc_;
public:
    using iterator = Iterator<T>;

    Vector()
        :   size_(0)
        ,   capacity_(1)
        ,   data_(alloc_.allocate(capacity_))
    {}

    ~Vector() {
        for (size_t i = 0; i < size_; ++i) {
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_);
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_t size() const noexcept {
        return size_;
    }

    size_t capacity() const noexcept {
        return capacity_;
    }

    auto begin() const noexcept {
        return iterator(data_);
    }

    auto end() const noexcept {
        return iterator(data_ + size_);
    }

    auto rbegin() const noexcept {
        return std::reverse_iterator<iterator>(data_ + size_);
    }

    auto rend() const noexcept {
        return std::reverse_iterator<iterator>(data_);
    }


    void push_back(T&& item) {
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        alloc_.construct(data_ + size_++, std::move(item));
    }

    void push_back(const T& item) {
        if (size_ == capacity_) {
            reserve(capacity_ * 2);
        }
        alloc_.construct(data_ + size_++, item);
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("");
        }
        alloc_.destroy(data_ + --size_);
    }

    void reserve(size_t n) {
        if (capacity_ < n) {
            T* tmp = alloc_.allocate(n);
            for (size_t i = 0; i < size_; ++i) {
                alloc_.construct(tmp + i, std::move(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_);
            data_ = tmp;
            capacity_ = n;
        }
    }

    void resize(size_t n) {
        if (size_ > n) {
            for (size_t i = n; i < size_; ++i) {
                alloc_.destroy(data_ + i);
            }
        } else {
            if (n > capacity_) {
                reserve(n);
            }
            for (size_t i = size_; i < n; ++i) {
                alloc_.construct(data_ + i);
            }
        }
        size_ = n;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            alloc_.destroy(data_ + i);
        }
        size_ = capacity_ = 0;
    }

    const T& operator[](size_t n) const {
        if (n >= size_) {
            throw std::out_of_range("");
        }
        return data_[n];
    }

    T& operator[](size_t n) {
        if (n >= size_) {
            throw std::out_of_range("");
        }
        return data_[n];
    }

};
