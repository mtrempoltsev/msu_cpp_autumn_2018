#include <stdexcept>
#include <iterator>


template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type size) {
        return static_cast<pointer>(::operator new(size * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type size) {
        ::operator delete[](ptr, sizeof(value_type) * size);
    }

    template<class... Args>
    void construct(pointer ptr, Args&& ... args) {
        ::new (ptr) T(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }
};


template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T> {
public:
    using pointer = T*;
    using reference = T&;
    using iterator = Iterator<T>;
    typedef std::random_access_iterator_tag iterator_category;

    explicit Iterator(pointer ptr) noexcept: ptr_(ptr)
    { }
    Iterator(const iterator& iter): ptr_(iter.ptr_)
    { }

    reference operator*() const noexcept{
        return *ptr_;
    }

    bool operator==(const iterator& other) const noexcept{
        return ptr_ == other.ptr_;
    }

    bool operator!=(const iterator& other) const noexcept{
        return !(*this == other);
    }

    iterator& operator++() noexcept{
        ++ptr_;
        return *this;
    }
    
    iterator& operator--() noexcept{
        --ptr_;
        return *this;
    }
private:
    pointer ptr_;
};


template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;

    explicit Vector() 
        : size_(0)
        , capacity_(8)
    {
        data_ = alloc_.allocate(capacity_);
    }

    explicit Vector(const size_type size)
        : size_(size)
        , capacity_(size * 2) 
    {
        for (auto ptr = data_; ptr != data_ + size_; ptr++)
                alloc_.construct(ptr);
    }

    ~Vector() {
        clear();
        alloc_.deallocate(data_, capacity_);
    }

    reference operator[](const size_type pos) {
        if (pos < size_)
            return data_[pos];

        throw std::out_of_range("Wrong index");
    }

    const_reference operator[](const size_type pos) const {
        if (pos < size_)
            return data_[pos];

        throw std::out_of_range("Wrong index");
    }

    void push_back(value_type&& value) {
        if (size_ >= capacity_)
            reserve((capacity_ > 0) ? (2 * capacity_) : 8);

        alloc_.construct(data_ + size_++, std::move(value));
    }

    void push_back(const_reference value) {
        if (size_ >= capacity_)
            reserve((capacity_ > 0) ? (2 * capacity_) : 8);

        alloc_.construct(data_ + size_++, value);
    }

    void pop_back() {
        if (size_ > 0) {
            alloc_.destroy(data_ + size_);
            size_--;
            return;
        }
        
        throw std::out_of_range("Empty vector");
    }

    bool empty() const noexcept{
        return (size_ == 0);
    }

    size_type size() const noexcept{
        return size_;
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    void clear() {
        for (size_type i = 0; i < size_; i++)
            alloc_.destroy(data_ + i);

        size_ = 0;
        capacity_ = 0;
    }

    iterator begin() const noexcept {
        return iterator(data_);
    }

    iterator end() const noexcept {
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin() const noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() const noexcept {
        return reverse_iterator(begin());
    }

    void resize(const size_type size) {
        if (size_ > size) {
            for (size_type i = size; i < size_; i++)
                alloc_.destroy(data_ + i); 
        } else {
            if (size > capacity_)
                reserve(size);

            for (size_type i = size_; i < size; i++)
                alloc_.construct(data_ + i);  
        }

        size_ = size;
    }

    void reserve(const size_type size) {
        if (size > capacity_) {
            pointer new_data = alloc_.allocate(size);
            for (size_type i = 0; i < size_; i++) {
                alloc_.construct(new_data + i, *(data_ + i));
                alloc_.destroy(data_ + i);
            }

            alloc_.deallocate(data_, capacity_);
            capacity_ = size;
            data_ = new_data;           
        }   
    }

private:
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type capacity_;
};
