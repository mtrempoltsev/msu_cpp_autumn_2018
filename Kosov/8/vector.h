#include <limits>


template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    
    pointer allocate(size_type count){
        return static_cast<pointer>(::operator new(count * sizeof(value_type)));
    }
    void deallocate(pointer ptr, size_type count){
        ::operator delete(ptr);
    }
    template<class... Args>
    void construct(pointer ptr, Args&&... args){
        ::new(ptr) T(std::forward<Args>(args)...);
    }
    void destroy(pointer ptr){
        ptr->~value_type();
    }
    size_type max_size() const noexcept{
        return std::numeric_limits<size_type>::max();
    }
};

template <class T>
class Iterator{
    T* ptr_;
    bool forw;
public:
    using reference = T&;
    using pointer = T*;
    
    explicit Iterator(pointer ptr, bool forw = true)
    : ptr_(ptr), forw(forw){}
    
    void operator=(pointer ptr){
        ptr_ = ptr;
    }
    
    bool operator==(const Iterator<T> &other) const{
        return ptr_ == other.ptr_;
    }
    
    bool operator!=(const Iterator<T> &other) const{
        return !(*this == other);
    }
    
    reference operator*() const{
        return *ptr_;
    }
    Iterator operator+(size_t n) const{
        return Iterator(ptr_ + n);
    }
    Iterator& operator++(){
        ptr_ = forw ? ptr_ + 1 : ptr_ - 1;
        return *this;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector{
private:
    T *arr;
    size_t size_;
    size_t max_size_;
    Alloc alloc_;
public:
    using iterator = Iterator<T>;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;
    
    Vector()
    : arr(nullptr), size_(0), max_size_(0){}
    Vector(size_type N)
    : arr(alloc_.allocate(2 * N)), size_(N),
    max_size_(2 * N){
        for(auto i = 0; i < size_; i++)
            alloc_.construct(arr + i);
    }
    ~Vector(){
        clear();
        alloc_.deallocate(arr, max_size_);
    }
    void clear(){
        for(auto i = 0; i < size_; ++i){
            alloc_.destroy(arr + i);
        }
        size_ = 0;
        max_size_ = 0;
    }
    reference operator[](int i){
        if(i < size_)
            return arr[i];
    }
    size_type size() const{
        return size_;
    }
    bool empty(){
        return (size_ == 0);
    }
    void resize(size_type size){
        if (size_ < size){
            reserve(2 * size);
            for (auto i = size_; i < size; i++) {
                alloc_.construct(arr + i);
            }
            size_ = size;
        }else if(size_ > size) {
            for (auto i = size; i < size_; i++) {
                alloc_.destroy(arr + i);
            }
            size_ = size;
        }
    }
    void reserve(size_type new_cap){
        if (max_size_ < new_cap){
            size_type new_max_size_ = new_cap;
            pointer buf = alloc_.allocate(new_cap);
            for(auto i = 0; i < size_; i++){
                alloc_.construct(buf + i, std::forward<value_type>(*(arr + i)));
                alloc_.destroy(arr + i);
            }
            alloc_.deallocate(arr, max_size_);
            max_size_ = new_max_size_;
            arr = buf;
        }
    }
    size_type capacity() const{
        return max_size_;
    }
    void pop_back(){
        if (size_ > 0){
            alloc_.destroy(arr + size_);
            --size_;
        }
    }
    void push_back(const T& new_el){
        if (size_ >= max_size_){
            size_type new_cap = (max_size_ > 0) ? (2 * max_size_) : 256;
            reserve(new_cap);
        }
        alloc_.construct(arr + size_, (new_el));
        size_++;
    }
    void push_back(T&& new_el){
        if (size_ >= max_size_){
            size_type new_cap = (max_size_ > 0) ? (2 * max_size_) : 256;
            reserve(new_cap);
        }
        alloc_.construct(arr + size_, std::forward<value_type>(new_el));
        size_++;
    }
    
    iterator begin(){
        return iterator(arr);
    }
    iterator end(){
        return iterator(arr + size_);
    }
    iterator rbegin(){
        return iterator(arr + size_ - 1, false);
    }
    iterator rend(){
        return iterator(arr - 1, false);
    }
};
