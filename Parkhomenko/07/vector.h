#include <iostream>
#include <limits>

template <class T>
class Allocator {
public:
    using pointer = T*;

    pointer allocate(size_t count) {
        return static_cast<pointer>(::operator new(count * sizeof(T)));
    }

    void deallocate(pointer ptr, size_t count) {
        ::operator delete(ptr);
    }

    template <class... Args>
    void construct(pointer ptr, Args&&... args) {
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
    using reference = T&;
private:
    pointer p_ptr;
    bool is_forward;
public:
    explicit Iterator(pointer ptr, bool is_forw) : p_ptr(ptr), is_forward(is_forw) {}

    Iterator& operator++() {
        p_ptr = is_forward ? p_ptr + 1 : p_ptr - 1;
        return *this;
    }

    bool operator==(const Iterator<T> &other) const {
        return (p_ptr == other.p_ptr);
    }
   
    bool operator!=(const Iterator<T> &other) const {
        return !(*this == other);
    }

    reference operator*() const {
        return *p_ptr;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using pointer = T*;
    using reference = T&;
    using iterator = Iterator<T>;
    using const_reference = const T&;
private:
    Alloc alloc_;
    pointer data_;
    size_t size_;
    size_t capacity_;
public:
    Vector() : alloc_(), data_(nullptr), size_(0), capacity_(0) {}

    Vector(size_t size) : alloc_(), size_(size), capacity_(size) {
        data_ = alloc_.allocate(size);
        for (size_t i = 0; i < size_; ++i) {
            alloc_.construct(data_ + i);
        }
    }

    ~Vector() {
        clear();
        alloc_.deallocate(data_, capacity_);
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            alloc_.destroy(data_ + i);
        }
        size_ = 0;
        capacity_ = 0;
    }

    size_t size() const {
        return size_;
    }
   
    size_t capacity() {
        return capacity_;
    }
    bool empty() {
        return (size_ == 0); 
    }

    void resize(size_t size) {
        if (size_ < size) {
            reserve(2 * size);
            for (size_t i = size_; i < size; ++i) {
                alloc_.construct(data_ + i);
            }
            size_ = size;
        } else {
            if (size_ > size) {
                for (size_t i = size; i < size_; ++i) {
                    alloc_.destroy(data_ + i);
                }
                size_ = size;
            }
        }
    }

    void reserve(size_t new_capacity) {
        if (capacity_ < new_capacity) {
            pointer new_data = alloc_.allocate(new_capacity);
            for (size_t i = 0; i < size_; ++i) {
                alloc_.construct(new_data + i, std::forward<T>(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_, capacity_);
            capacity_ = new_capacity;
            data_ = new_data;
        }
    }

    void pop_back() {
        if (size_ > 0) {
            alloc_.destroy(data_ + size_--);
        }
    }
 
    void push_back(const_reference value) {
        if (size_ >= capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 256 : (2 * capacity_);
            reserve(new_capacity);
        }
        alloc_.construct(data_ + size_, value);
        ++size_;
    }

    reference operator[](int i) {
        if (i < size_) {
            return data_[i];
        }
    }
  
    iterator begin() {
        return iterator(data_, true);
    }
 
    iterator end() {
        return iterator(data_ + size_, true);
    }

    iterator rbegin() {
        return iterator(data_ + size_ - 1, false);
    }

    iterator rend() {
        return iterator(data_ - 1, false);
    }

};



