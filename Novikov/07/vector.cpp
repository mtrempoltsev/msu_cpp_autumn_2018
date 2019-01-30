#include <iterator>
#include <iostream>

template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    pointer allocate(size_type len) {
        return static_cast<pointer>(::operator new(len*sizeof(value_type)));
    }
    void deallocate(pointer ptr, size_type len) {
        ::operator delete(ptr);
    }
    template <class... Args>
    void construct(pointer ptr, Args&&... args) {
        ::new(ptr) T(std::forward<Args>(args)...);
    }
    void destroy(pointer ptr) {
        ptr->~value_type();
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
    T* ptr_;
public:
    using reference = T&;
    using pointer = T*;
    explicit Iterator(pointer ptr): ptr_(ptr) {}
    reference operator*() const{
        return *ptr_;
    }
    bool operator==(const Iterator<T> &other) const {
        return ptr_ == other.ptr_;
    }
    bool operator!=(const Iterator<T> &other) const {
        return !(*this == other);
    }    
    Iterator& operator++() {
        ++ptr_;
        return *this;
    }
    Iterator& operator--() {
        --ptr_;
        return *this;
    }
    bool operator>(const Iterator<T> &other) const {
        return distance(other - ptr_) > 0;
    }
    bool operator>=(const Iterator<T> &other) const {
        return distance(other - ptr_) >= 0;
    }
    bool operator<(const Iterator<T> &other) const {
        return distance(other - ptr_) < 0;
    }
    bool operator<=(const Iterator<T> &other) const {
        return distance(other - ptr_) <= 0;
    }
    Iterator& operator+=(size_t k) {
        for(size_t i = 0; i < k; i++) ptr_++;
        return *this;
    }
    Iterator& operator-=(size_t k) {
        for(size_t i = 0; i < k; i++) ptr_++;
        return *this;    
    }
    Iterator operator+(size_t n) const {
        return Iterator(ptr_ + n);
    }
    Iterator operator-(size_t n) const {
        return Iterator(ptr_ - n);
    }
    void operator=(Iterator iterator) {
        ptr_ = iterator.p;
    }
    void operator=(pointer ptr) {
        ptr_ = ptr;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector {
private:
    size_t size_;
    T* data;
    Alloc alloc_;
    size_t max_size_;
    static const size_t BUF_SIZE = 256;
public:
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using const_reference = const T&;
    using iterator = Iterator<T>;

/*  explicit Vector(): size_(0), max_size_(2) {
        data = alloc_.allocate(max_size_);
    } */
    explicit Vector(size_type N = 0): size_(N), max_size_(2 * N) {
            data = alloc_.allocate(max_size_);
            for(size_type i = 0; i < size_; i++)
                alloc_.construct(data + i);
    }
    void clear() noexcept {
        for(auto i = 0; i < size_; ++i) alloc_.destroy(data + i);
        size_ = 0;
        max_size_ = 0;
    }
    reference operator[](size_type i) {
        return data[i];
    }
    const_reference operator[](size_type i) const {
        return data[i];
    }
    size_type size() const noexcept {
        return size_;
    }
    bool empty() const noexcept {
        return size_ == 0;
    }
    void reserve(size_t new_cap) {
        if(max_size_ < new_cap) {
            pointer buf = alloc_.allocate(new_cap);
            for(auto i = 0; i < size_; i++) {
                alloc_.construct(buf + i, std::forward<value_type>(*(data + i)));
                alloc_.destroy(data + i);
            }
            alloc_.deallocate(data, max_size_);
            max_size_ = new_cap;
            data = buf;
        }
    }
    void resize(size_t size) {
        if(size_ < size){
            reserve(size << 1);
            for(size_type i = size_; i < size; i++) alloc_.construct(data + i);
            size_ = size;
        }
        else if(size_ > size) {
            for(size_type i = size; i < size_; i++) alloc_.destroy(data + i);
            size_ = size;
        }
    }
    size_type capacity() const noexcept {
        return max_size_;
    }
    void pop_back() {
        if(size_) {
            alloc_.destroy(data + size_);
            --size_;
        }
    }
    void push_back(const T& new_el) {
        if(size_ >= max_size_) {
            size_type new_cap = (max_size_ > 0) ? (2 * max_size_) : BUF_SIZE;
            reserve(new_cap);
        }
        alloc_.construct(data + size_, (new_el));
        ++size_;
    }
    void push_back(T&& new_el) {
        if(size_ >= max_size_) {
            size_type new_cap = (max_size_ > 0) ? (2 * max_size_) : BUF_SIZE;
            reserve(new_cap);
        }
        alloc_.construct(data + size_, std::move(new_el));
        ++size_;
    }
    iterator begin() {
        return iterator(data);
    }
    iterator end() {
        return iterator(data + size_);
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    ~Vector() {
        clear();
        alloc_.deallocate(data, max_size_);
    }
};
