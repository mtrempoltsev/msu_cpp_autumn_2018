#include <exception>
#include <iterator>
using namespace std;

template<class T>
class Allocator{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    pointer allocate(const int n_elem){
        pointer ptr = (pointer) malloc(sizeof(value_type)* n_elem);
        return ptr;
    }
    template <class... Args>
	void construct(pointer ptr, Args&&... arg){
	    ptr = new(ptr) value_type(forward<Args>(arg)...);
	}
	void destroy(pointer ptr){
	    ptr->~value_type();
	}
	void deallocate(pointer ptr){
        free(ptr);
	}
};

template <class T>
class Iterator: public iterator<random_access_iterator_tag, T>
{
    T* ptr;
public:
    using reference = T&;
    using pointer = T*;
    using size_type = size_t;

    Iterator(){}

    Iterator(pointer other){
        ptr = other;
    }

    bool operator == (const Iterator<T>& other) const{
        return (ptr == other.ptr);
    }
    bool operator != (const Iterator<T>& other) const{
        return !(ptr == other.ptr);
    }
    reference operator*(){
        return *ptr;
    }
    Iterator<T>& operator++(){
        ptr ++;
        return *this;
    }
    Iterator<T>& operator-- (){
        ptr --;
        return *this;
    }
    Iterator<T>& operator+=(size_type n) {
        ptr += n;
        return *this;
    }
    Iterator<T>& operator-=(size_type n) {
        ptr -= n;
        return *this;
    }
    Iterator<T> operator+(size_type n) const {
        return Iterator<T>(ptr + n);
    }
    Iterator operator-(size_type n) const {
        return Iterator<T>(ptr - n);
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
private:
    size_t size_;
    size_t capacity_;
    T* data_;
    Alloc alloc_;
public:
    using iterator = Iterator<T>;
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
	using allocator_type = Alloc;

	Vector(): size_(0), capacity_(1), data_(alloc_.allocate(capacity_))
    {}

    ~Vector(){
        for (size_type i = 0; i < size_; i++){
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_);
    }

    bool empty() const noexcept{
        return (size_ == 0);
    }

    size_type size() const noexcept{
        return size_;
    }

    size_type capacity() const noexcept{
        return capacity_;
    }

    auto begin() const noexcept{
        return iterator(data_);
    }

    auto end() const noexcept{
        return iterator(data_ + size_);
    }

    auto rbegin() const noexcept{
        return reverse_iterator<iterator>(data_ + size_);
    }

    auto rend() const noexcept{
        return reverse_iterator<iterator>(data_);
    }

    void push_back(T&& val){
        if (size_ == capacity_)
            reserve(capacity_ * 2);
        alloc_.construct(data_ + size_, move(val));
        size_++;
    }

    void push_back(const_reference val){
        if (size_ == capacity_)
            reserve(capacity_ * 2);
        alloc_.construct(data_ + size_, val);
        size_++;
    }

    void pop_back(){
        if (size_ == 0)
            throw out_of_range("");
        size_--;
        alloc_.destroy(data_ + size_);
    }

    void reserve(size_t n){
        if (capacity_ < n){
            pointer p = alloc_.allocate(n);
            for (size_type i = 0; i < size_; i++){
                alloc_.construct(p + i, move(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_);
            data_ = p;
            capacity_ = n;
        }
    }

    void resize(size_type n){
        if (size_ >= n){
            for (size_type i = n; i < size_; i++){
                alloc_.destroy(data_ + i);
            }
        }else{
            if (n > capacity_)
                reserve(n);
            for (size_type i = size_; i < n; i++)
            {
                alloc_.construct(data_ + i);
            }
        }
        size_ = n;
    }

    void clear(){
        for (size_type i = 0; i < size_; i++){
            alloc_.destroy(data_ + i);
        }
        size_ = 0;
        capacity_ = 0;
    }

    reference operator[](size_type n){
        if (n > size_)
            throw out_of_range("");
        return data_[n];
    }
    const_reference operator[](size_type n) const{
        if (n > size_)
            throw out_of_range("");
        return data_[n];
    }
};
