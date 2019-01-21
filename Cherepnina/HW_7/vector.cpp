#include <iostream>

template<class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T *;
    using size_type = size_t;

    pointer allocate(size_type n) {
        return static_cast<pointer>(::operator new(sizeof(value_type) * n));
    }

    void deallocate(pointer p) {
        ::operator delete(p);
    }

    template<class... ArgsT>
    void construct(pointer p, ArgsT &&... args) {
        ::new(p) T(std::forward<ArgsT>(args)...);
    }

    void destroy(pointer p) {
        p->~value_type();
    }
};

template<class T>
class Iterator {
    T *current_;
    int direction;

    void findNext() {
            current_ += direction;
    }

public:
    Iterator(T *first, int direction = 1) : current_(first),
                                                     direction(direction) {}

    bool operator==(const Iterator &other) const {
        return *current_ == *other.current_;
    }

    bool operator!=(const Iterator &other) const {
        return !(*this == other);
    }

    void operator++() {
            findNext();
    }

    T operator*() const {
        return *current_;
    }

};

template<class T, class Alloc = Allocator<T>>
class Vector {

public:
    using iterator = Iterator<T>;
    explicit Vector() : size_(0), capacity_(initial_length) {
        data = alloc_.allocate(capacity_);
    }

    ~Vector() {
        clear();
        alloc_.deallocate(data);
    }

    T &operator[](int num) {
        return data[num];
    }

    const T &operator[](int num) const{
        return data[num];
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() {
        return capacity_;
    }

    void push_back(T &&arg) {
        if (size_ == capacity_)
            reserve(capacity_ << 1);
        alloc_.construct(data + size_, std::move(arg));
        ++size_;
    }

    void push_back(const T &arg) {
        if (size_ == capacity_)
            reserve(capacity_ << 1);
        alloc_.construct(data + size_, arg);
        ++size_;
    }

    void pop_back() {
        --size_;
        alloc_.destroy(data + size_);
    }

    void reserve(size_t new_size) {
        if (new_size > capacity_) {
            T *ndata = alloc_.allocate(new_size);
            for (size_t i = 0; i < size_; ++i) {
                alloc_.construct(ndata + i, *(data + i));
                alloc_.destroy(data + i);
            }
            alloc_.deallocate(data);
            data = ndata;
            capacity_ = new_size;
        }
    }

    void resize(size_t count) {
        if (size_ > count) {
            for (size_t i = count; i < size_; ++i) {
                alloc_.destroy(data + i);
            }
        } else {
            if (capacity_ < count)
                this->reserve(count << 1);
            for (size_t i = size_; i < count; i++)
                alloc_.construct(data + i);
        }
        size_ = count;
    }

    void clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            alloc_.destroy(data + i);
        }
        size_ = 0;
        capacity_ = 0;
    }

    iterator rbegin() const {
        return iterator(&data[size_ - 1], -1);
    }

    iterator rend() const {
        return iterator(&data[-1]);
    }

    iterator begin() const {
        return iterator(&data[0], 1);
    }

    iterator end() {
        return iterator(&data[size_]);
    }

private:
    const size_t initial_length = 2;
    Alloc alloc_;
    T* data;
    size_t size_;
    size_t capacity_;
};
