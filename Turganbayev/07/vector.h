#include <iterator>

template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
public:
    pointer allocate(size_type size) {
        return static_cast<pointer>(::operator new(size * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type size) {
        ::operator delete[](ptr, size * sizeof(value_type));
    }

    template <typename... ArgsT>
    void construct(pointer ptr, ArgsT&&... args) {
        ::new (ptr) value_type(std::forward<ArgsT>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }
};

template<class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T> {
public:
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;
private:
    pointer ptr_;
public:
    explicit Iterator(pointer ptr) : ptr_(ptr) {}

    reference operator *() const {
        return *ptr_;
    }

    bool operator < (const Iterator<T> rhs) const {
        return ptr_ < rhs.ptr_;
    }

    bool operator >= (const Iterator<T> rhs) const {
        return ptr_ >= rhs.ptr_;
    }

    bool operator > (const Iterator<T> rhs) const {
        return ptr_ > rhs.ptr_;
    }

    bool operator <= (const Iterator<T> rhs) const {
        return ptr_ <= rhs.ptr_;
    }

    bool operator == (const Iterator<T> rhs) const {
        return ptr_ == rhs.ptr_;
    }

    bool operator != (const Iterator<T> rhs) const {
        return ptr_ != rhs.ptr_;
    }

    Iterator& operator++() {
        ptr_++;
        return *this;
    }

    Iterator& operator+=(size_type size) {
        ptr_ += size;
        return *this;
    }

    Iterator& operator--() {
        ptr_--;
        return *this;
    }

    Iterator& operator-=(size_type size) {
        ptr_ -= size;
        return *this;
    }
};

template<class T, class Alloc = Allocator<T>>
class Vector {
public:
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<value_type>;
    using rvalueref = T&&;
    using const_rvalueref = const T&&;
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;
private:
    Alloc alloc_;
    size_type size_;
    size_type capacity_;
    pointer data_;
    static const size_type default_size = 16;
public:
    explicit Vector(size_type n) : size_(n),
            capacity_(n) {
        data_ = alloc_.allocate(capacity_);
        for (size_t i = 0; i < size; i++) {
            alloc_.construct(data_ + i);
        }
    }

    explicit Vector() : size_(0), capacity_(default_size) {
        data_ = alloc_.allocate(capacity_);
    }

    size_type size() const {
        return size_;
    }

    size_type capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        for (size_t i = 0; i < size_; i++) {
            alloc_.destroy(data_ + i);
        }
        size_ = 0;
    }

    ~Vector() {
        clear();
        alloc_.deallocate(data_, capacity_);
        capacity_ = 0;
    }

    void reserve(size_type new_capacity) {
        if (capacity_ < new_capacity) {
            pointer new_data = alloc_.allocate(new_capacity);
            for (size_t i = 0; i < size_; i++) {
                alloc_.construct(new_data + i,
                    std::forward<value_type>(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    void resize(size_type new_size) {
        if (new_size < size_) {
            for (size_t i = new_size; i < size_; i++) {
                alloc_.destroy(data_ + i);
            }
            size_ = new_size;
        } else {
            if (new_size > capacity_) {
                reserve(new_size);
            }
            for (size_t i = size_; i < new_size; i++) {
                alloc_.construct(data_ + i);
            }
            size_ = new_size;
        }
    }

    void push_back(const_reference value) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? default_size : 2 * capacity_;
            reserve(new_capacity);
        }
        alloc_.construct(data_ + size_, value);
        size_++;
    }

    void push_back(rvalueref value) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? default_size : 2 * capacity_;
            reserve(new_capacity);
        }
        alloc_.construct(data_ + size_, std::move(value));
        size_++;
    }

    void pop_back() {
        if (!empty()) {
            alloc_.destroy(data_  + size_);
            size_--;
        } else {
            throw std::out_of_range("empty vector");
        }
    }

    reference operator[] (size_type idx) {
        if (idx < size_) {
            return data_[idx];
        } else {
            throw std::out_of_range("invalid idx");
        }
    }

    const_reference operator[] (size_type idx) const {
        if (idx < size_) {
            return data_[idx];
        } else {
            throw std::out_of_range("invalid idx");
        }
    }

    iterator begin() const {
        return iterator(data_);
    }

    iterator end() const {
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(end());
    }

    reverse_iterator rend() const {
        return reverse_iterator(begin());  
    }
};