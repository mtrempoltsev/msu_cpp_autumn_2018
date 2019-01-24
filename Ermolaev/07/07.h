#include <iostream>
#include <cstdlib>
#include <limits>

template <class T>
class Allocator {
public:
    using pointer = T*;

    pointer allocate(size_t cnt) {
        return static_cast<pointer>(::operator new(cnt * sizeof(T)));
    }

    void deallocate(pointer ptr, size_t cnt) {
        ::operator delete(ptr);
    }

    template <class... Args>
    void construct(T *ptr, Args&&... args) {
        ::new (ptr) T(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~T();
    }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max();
    }
};


template <class T>
class Iterator {
public:
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
class Vector {
public:
    using iterator = Iterator<T>;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;

private:
    Alloc alloc_;
    pointer data_;
    size_t len_;
    size_t size_;
    static const size_t BASE_SIZE = 256;

public:

    Vector () : len_(0), alloc_(), size_(BASE_SIZE) {
        data_ = alloc_.allocate(size_);
    }

    Vector (size_t size, const T& def) : len_(size), alloc_(), size_(size) {
        data_ = alloc_.allocate(size_);
        for (size_t i = 0; i < len_; ++i) {
            alloc_.construct(data_ + i, def);
        }
    }

    ~Vector() {
        for (size_t i = 0; i < len_; ++i) {
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, size_);
    }

    T& operator[] (size_t ind) {
        return data_[ind];
    }

    const reference operator[] (size_t ind) const {
        return data_[ind];
    }

    void reserve(size_t newSize) {
        if (size_ < newSize)
        {
            auto newData = alloc_.allocate(newSize);

            pointer cur_data = data_, cur_new = newData;
            for (size_t i = 0; i < size_; ++i, ++cur_data, ++cur_new) {
                alloc_.construct(cur_new, *cur_data);
            }

            alloc_.deallocate(data_, size_);
            data_ = newData;
            size_ = newSize;
        }
    }

    void resize(size_t newSize, T&& def = T()) {
        if (len_ < newSize)
        {
            this->reserve(newSize * 2);
            for (size_t i = len_; i < newSize; ++i) {
                alloc_.construct(data_ + i, def);
            }
            len_ = newSize;
        } else if (len_ > newSize) {
            for (size_t i = newSize; i < len_; ++i) {
                alloc_.destroy(data_ + i);
            }
            len_ = newSize;
        }
    }

    void push_back(const T& value) {
        if (len_ == size_) {
            this->reserve(size_ * 2);
        }
        alloc_.construct(data_ + len_, value);
        ++len_;
    }

    void pop_back() {
        alloc_.destroy(data_ + len_ - 1);
        --len_;
    }

    bool empty() const {
        return len_ == 0;
    }

    size_t size() const {
        return len_;
    }

    size_t capacity() const {
        return size_;
    }

    void clear() {
        for (size_t i = 0; i < len_; ++i) {
            alloc_.destroy(data_ + i);
        }
        len_ = 0;
    }

    Iterator<T> begin() {
        return Iterator<T>(data_, true);
    }

    Iterator<T> end() {
        return Iterator<T>(data_ + len_, true);
    }

    Iterator<T> rbegin() {
        return Iterator<T>(data_ + len_ - 1, false);
    }

    Iterator<T> rend() {
        return Iterator<T>(data_ - 1, false);
    }
};
