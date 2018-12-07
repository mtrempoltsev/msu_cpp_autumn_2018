#include <iterator>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type n){
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type n){
        ::operator delete(p, n * sizeof(value_type));
    }

    template <class... Args>
    void construct(pointer p, Args&&... args){
        ::new(p) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer p){
        p->~value_type();
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator = Iterator<value_type>;

private:
    pointer current_;

public:
    explicit Iterator(pointer p) : current_(p) {}
    Iterator(const iterator& it) : current_(it.current_) {}

    bool operator==(const iterator& other){
        return current_ == other.current_;
    }

    bool operator!=(const iterator& other){
        return !(*this == other);
    }

    bool operator<(const iterator& other){
        return current_ < other.current_;
    }

    bool operator>(const iterator& other){
        return other < *this;
    }

    bool operator>=(const iterator& other){
        return !(*this < other);
    }

    bool operator<=(const iterator& other){
        return !(*this > other);
    }

    reference operator*(){
        return *current_;
    }

    iterator& operator++(){
        ++current_;
        return *this;
    }

    iterator& operator--(){
        --current_;
        return *this;
    }

    iterator& operator+=(size_t n){
        current_ += n;
        return *this;
    }

    iterator& operator-=(size_t n){
        current_ -= n;
        return *this;
    }

    iterator operator+(size_t n){
        iterator tmp(current_ + n);
        return tmp;
    }

    iterator operator-(size_t n){
        iterator tmp(current_ - n);
        return tmp;
    }

    iterator operator+(const iterator& other){
        iterator tmp(current_ + other.current_);
        return tmp;
    }

    iterator operator-(const iterator& other){
        iterator tmp(current_ - other.current_);
        return tmp;
    }

    friend iterator operator+(size_t n, const iterator& other){
        iterator tmp(other.current_ + n);
        return tmp;
    }

    friend iterator operator-(size_t n, const iterator& other){
        iterator tmp(other.current_ - n);
        return tmp;
    }

    reference operator[](size_t n){
        return current_[n];
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;
    using size_type = size_t;
    using iterator = Iterator<value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    
private:
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type allocated_size_;

public:

    explicit Vector(size_type n = 0)
        : data_(alloc_.allocate(n)),
        size_(n), allocated_size_(n) 
    {
        for (size_type i = 0; i < size_; ++i)
            alloc_.construct(data_ + i);
    }

    ~Vector(){
        clear();
        alloc_.deallocate(data_, allocated_size_);
    }

    reference operator[] (size_type i){
        return data_[i];
    }

    const_reference operator[] (size_type i) const{
        return data_[i];
    }

    void pop_back(){
        --size_;
        alloc_.destroy(data_ + size_);
    }

    void push_back(value_type &&value){
        if (size_ >= allocated_size_)
            reserve(2 * allocated_size_ + 1);
        alloc_.construct(data_ + size_, std::move(value));
        ++size_;
    }

    void push_back(const value_type& value){
        if (size_ >= allocated_size_)
            reserve(2 * allocated_size_ + 1);
        alloc_.construct(data_ + size_, value);
        ++size_;
    }

    size_type size() const{
        return size_;
    }

    size_type capacity(){
        return allocated_size_;
    }

    bool empty() const{
        return size_ == 0;
    }

    iterator begin(){
        return iterator(data_);
    }

    iterator end(){
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin(){
        return reverse_iterator(end());
    }

    reverse_iterator rend(){
        return reverse_iterator(begin());
    }

    void reserve(size_type new_cap){
        if (new_cap <= size_)
            return;

        pointer tmp = alloc_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i){
            alloc_.construct(tmp + i, data_[i]);
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, allocated_size_);

        data_ = tmp;
        allocated_size_ = new_cap;
    }

    void clear(){
        while (size_ > 0){
            alloc_.destroy(data_ + size_);
            --size_;
        }
    }

    void resize(size_type n){
        if (n < size_){
            for (pointer p = data_ + n; p != data_ + size_; ++p){
                alloc_.destroy(p);
            }
            size_ = n;
            return;
        }

        if(n >= allocated_size_)
            this->reserve(n);

        for (pointer p = data_ + size_; p != data_ + n; ++p) {
            alloc_.construct(p);
        }
        size_ = n;
        return;
    }
};