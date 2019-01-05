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
    T *end_;
    int direction;

    void findNext() {
        if (current_ != end_)
            current_ += direction;
    }

public:
    Iterator(T *first, T *last, int direction = 1) : current_(first),
                                                     end_(last),
                                                     direction(direction) {}

    bool operator==(const Iterator &other) const {
        return *current_ == *other.current_;
    }

    bool operator!=(const Iterator &other) const {
        return !(*this == other);
    }

    void operator++() {
        if (current_ != end_) {
            findNext();
        }
    }

    T operator*() const {
        return *current_;
    }

};

template<class T, class Alloc = Allocator<T>>
class Vector {

public:
    using iterator = Iterator<T>;
    explicit Vector() : size_(0), capacity_(2) {
        data = alloc_.allocate(capacity_);
    }

    ~Vector() {
        clear();
        alloc_.deallocate(data);
    }

    T &operator[](int num) {
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
        return iterator(&data[size_ - 1], &data[-1], -1);
    }

    iterator rend() const {
        return iterator(&data[-1], &data[-1]);
    }

    iterator begin() const {
        return iterator(&data[0], &data[size_], 1);
    }

    iterator end() {
        return iterator(&data[size_], &data[size_]);
    }

private:
    Alloc alloc_;
    T* data;
    size_t size_;
    size_t capacity_;
};
