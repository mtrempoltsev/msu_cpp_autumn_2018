#include <limits>

template <typename T>
class Allocator {
public:
    using pT = T*;
    
    pT allocate(size_t count) {
        return static_cast <pT> (::operator new(count * sizeof(T)));
    }

    void deallocate(pT ptr, size_t count) {
        ::operator delete(ptr);
    }

    template <typename... Args>
    void construct(pT ptr, Args&&... args) {
        ::new(ptr) T(std::forward <Args> (args)...);
    }

    void destroy(pT ptr) {
        ptr->~T();
    }

    size_t max_size() const noexcept {
        return std::numeric_limits <size_t> ::max();
    }
};

template <typename T>
class Iterator {
    T* ptr_;
    bool forw;
public:
    using refT = T&;
    using pT = T*;
    
    explicit Iterator(pT ptr, bool forw = true) : ptr_(ptr), forw(forw) {
    }
    
    void operator=(pT ptr) {
        ptr_ = ptr;
    }
    
    bool operator==(const Iterator <T> &other) const {
        return ptr_ == other.ptr_;
    }
    bool operator!=(const Iterator <T> &other) const {
        return !(*this == other);
    }
    
    refT operator*() const {
        return *ptr_;
    }

    Iterator operator+(size_t n) const {
        return Iterator(ptr_ + n);
    }

    Iterator& operator++() {
        if (forw)
            ++ptr_;
        else 
            --ptr_;
        return *this;
    }
};

template <typename T, typename A = Allocator <T> >
class Vector {
    T *m_;
    size_t size_;
    size_t max_size_;
    A alloc_;
public:
    using it = Iterator <T>;
    using pT = T*;
    using refT = T&;
    
    Vector() : m_(nullptr), size_(0), max_size_(0) {
    }

    Vector(size_t N) : m_(alloc_.allocate(N << 1)), 
            size_(N), max_size_(N << 1) {
        for(size_t i = 0; i < size_; i++)
            alloc_.construct(m_ + i);
    }

    ~Vector() {
        clear();
        alloc_.deallocate(m_, max_size_);
    }

    void clear() {
        for(size_t i = 0; i < size_; ++i) {
            alloc_.destroy(m_ + i);
        }
        size_ = 0;
        max_size_ = 0;
    }

    refT operator[](int i) {
        if(i < size_)
            return m_[i];
    }

    size_t size() const {
        return size_;
    }
    
    bool empty() {
        return (size_ == 0);
    }

    void resize(size_t size) {
        if (size_ < size) {
            reserve(size << 1);
            for (size_t i = size_; i < size; i++) {
                alloc_.construct(m_ + i);
            }
            size_ = size;
        } else if(size_ > size) {
            for (size_t i = size; i < size_; i++) {
                alloc_.destroy(m_ + i);
            }
            size_ = size;
        }
    }

    void reserve(size_t new_cap) {
        if (max_size_ < new_cap) {
            size_t new_max_size_ = new_cap;
            pT buf = alloc_.allocate(new_cap);
            for(size_t i = 0; i < size_; i++) {
                alloc_.construct(buf + i, std::forward <T> (*(m_ + i)));
                alloc_.destroy(m_ + i);
            }
            alloc_.deallocate(m_, max_size_);
            max_size_ = new_max_size_;
            m_ = buf;
        }
    }

    size_t capacity() const {
        return max_size_;
    }

    void pop_back() {
        if (size_ > 0) {
            alloc_.destroy(m_ + size_);
            --size_;
        }
    }

    void push_back(const T& new_el) {
        if (size_ >= max_size_) {
            size_t new_cap = (max_size_ > 0) ? (max_size_ << 1) : 256;
            reserve(new_cap);
        }
        alloc_.construct(m_ + size_, (new_el));
        size_++;
    }

    void push_back(T&& new_el) {
        if (size_ >= max_size_) {
            size_t new_cap = (max_size_ > 0) ? (max_size_ << 1) : 256;
            reserve(new_cap);
        }
        alloc_.construct(m_ + size_, std::move(new_el));
        size_++;
    }
    
    it begin() {
        return it(m_);
    }

    it end() {
        return it(m_ + size_);
    }

    it rbegin() {
        return it(m_ + size_ - 1, false);
    }

    it rend() {
        return it(m_ - 1, false);
    }
};