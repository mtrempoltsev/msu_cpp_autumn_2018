#include <iterator>

template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type size) {
        return static_cast<pointer>(::operator new(sizeof(value_type) * size));
    }
    void deallocate(pointer ptr, size_t count) {
        ::operator delete(ptr);
    }
    template<class... Args>
    void construct(pointer p, Args&&... args){
        ::new(p) T(std::forward<Args>(args)...);
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
    
    explicit Iterator(pointer ptr)
    : ptr_(ptr)
    {}

    reference operator*() const{
        return *ptr_;
    }
    
    void operator=(Iterator iterator){
        ptr_ = iterator.p;
    }
    
    void operator=(pointer ptr) {
        ptr_ = ptr;
    }
    
    bool operator==(const Iterator<T> &other) const {
        return ptr_ == other.ptr_;
    }
    
    bool operator!=(const Iterator<T> &other) const {
        return !(*this == other);
    }
    
    Iterator operator+(size_t n) const{
        return Iterator(ptr_ + n);
    }
    
    Iterator operator-(size_t n) const {
        return Iterator(ptr_ - n);
    }
    
    Iterator& operator++(){
        ++ptr_;
        return *this;
    }
    
    Iterator& operator--(){
        --ptr_;
        return *this;
    }
    
    Iterator& operator+=(size_t n) {
        for(auto i = 0; i < n; ++i) ++ptr_;
        return *this;
    }
    
    Iterator& operator-=(size_t n) {
        for(auto i = 0; i < n; ++i) ++ptr_;
        return *this;    
    }
    
    bool operator>(const Iterator<T> &other) const {
        return distance(other - ptr_) > 0;
    }
    
    bool operator<(const Iterator<T> &other) const {
		return distance(other - ptr_) < 0;
	}
	
	bool operator>=(const Iterator<T> &other) const {
		return distance(other - ptr_) >= 0;
	}
	
	bool operator<=(const Iterator<T> &other) const {
		return distance(other - ptr_) <= 0;
	}
	
//    reference operator[](size_t i) {
  //      return p[i];
    //}
    
};


template <class T, class Alloc = Allocator<T>>
class Vector{
private:
    T *data;
    size_t size_;
    size_t max_size_;
    Alloc alloc_;
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;
    
    explicit Vector()
    : data(nullptr), 
    size_(0), 
    max_size_(0)
    {}
    
    explicit Vector(size_t N)
    : data(alloc_.allocate(2 * N)), 
    size_(N),
    max_size_(2 * N)
    {
        for(auto i = 0; i < size_; i++)
            alloc_.construct(data + i);
    }
    
    ~Vector() {
        clear();
        alloc_.deallocate(data, max_size_);
    }
    
    void clear() noexcept {
        for(auto i = 0; i < size_; ++i){
            alloc_.destroy(data + i);
        }
        size_ = 0;
        max_size_ = 0;
    }
    
    reference operator[](size_t i) {
        return data[i];
    }
    
    size_t size() const {
        return size_;
    }
    
    bool empty() {
        return size_ == 0;
    }
    
    void reserve(size_t new_cap) {
        if (max_size_ < new_cap){
            pointer buf = alloc_.allocate(new_cap);
            for(auto i = 0; i < size_; i++){
                alloc_.construct(buf + i, std::forward<value_type>(*(data + i)));
                alloc_.destroy(data + i);
            }
            alloc_.deallocate(data, max_size_);
            max_size_ = new_cap;
            data = buf;
        }
    }
    
    void resize(size_t size) {
        if (size_ < size){
            reserve(size << 1);
            for (auto i = size_; i < size; i++) {
                alloc_.construct(data + i);
            }
            size_ = size;
        }else if(size_ > size) {
            for (auto i = size; i < size_; i++) {
                alloc_.destroy(data + i);
            }
            size_ = size;
        }
    }
    
    size_type capacity() const {
        return max_size_;
    }
    
    void pop_back(){
        if (size_ > 0){
            alloc_.destroy(data + size_);
            --size_;
        }
    }
    
    void push_back(const T& new_el) {
        if (size_ >= max_size_){
            size_type new_cap = (max_size_ > 0) ? (2 * max_size_) : 256;
            reserve(new_cap);
        }
        alloc_.construct(data + size_, (new_el));
        ++size_;
    }
    
    void push_back(T&& new_el) {
        if (size_ >= max_size_){
            size_type new_cap = (max_size_ > 0) ? (2 * max_size_) : 256;
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
    
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
};
