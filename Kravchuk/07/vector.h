#include <iostream>
#include <cstdlib>


/* operator[], push_back, pop_back, empty, size, clear, begin, end, rbegin, rend, resize, reserve */

template <class T>
class Allocator {
  using value_type = T;
  using pointer = T*;
  using size_type = size_t;

public:
  pointer allocate (size_type count) {
    return (pointer)malloc(count * sizeof (value_type));
  }

  void deallocate (pointer ptr) {
    free (ptr);
  }

  template<class... Args>
  void constr (pointer ptr, Args&&... args) {
    new(ptr) value_type (std::forward<Args>(args)...);
  }

  void destr (pointer ptr) {
    ptr->~value_type ();
  }
};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T> {
  T *ptr_ = nullptr;
public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = Iterator<T>;

  Iterator (T *ptr): ptr_(ptr) {}

  bool operator== (const iterator& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator!= (const iterator& other) const {
    return !(*this == other);
  }

  bool operator< (const iterator& other) const {
    return ptr_ < other.ptr_;
  }

  bool operator> (const iterator& other) const {
    return ptr_ > other.ptr_;
  }

  bool operator<= (const iterator& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator>= (const iterator& other) const {
    return ptr_ >= other.ptr_;
  }

  reference operator* () const {
    return *ptr_;
  }

  iterator& operator++ () {
    ++ptr_;
    return *this;
  }

  iterator& operator-- () {
    --ptr_;
    return *this;
  }

  iterator operator+ (size_type n) {
    iterator tmp (ptr_ + n);
    return tmp;
  }

  iterator operator- (size_type n) {
    iterator tmp (ptr_ - n);
    return tmp;
  }

  iterator operator+(const iterator& other) {
     iterator tmp(ptr_ + other.ptr_);
     return tmp;
  }

  iterator operator-(const iterator& other) {
     iterator tmp(ptr_ - other.ptr_);
     return tmp;
  }

  iterator& operator+= (size_type n) {
    ptr_ += n;
    return *this;
  }

  iterator& operator-= (size_type n) {
    ptr_ -= n;
    return *this;
  }

  reference operator[](size_t count) {
     return ptr_[count];
  }

};

template <class T>
class ReverseIterator: public std::iterator<std::random_access_iterator_tag, T> {
  T *ptr_ = nullptr;
public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = ReverseIterator<T>;

  ReverseIterator (T *ptr): ptr_(ptr) {}

  bool operator== (const iterator& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator!= (const iterator& other) const {
    return !(*this == other);
  }

  bool operator< (const iterator& other) const {
    return ptr_ >= other.ptr_;
  }

  bool operator> (const iterator& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator<= (const iterator& other) const {
    return ptr_ > other.ptr_;
  }

  bool operator>= (const iterator& other) const {
    return ptr_ < other.ptr_;
  }

  reference operator* () const {
    return *ptr_;
  }

  iterator& operator++ () {
    --ptr_;
    return *this;
  }

  iterator& operator-- () {
    ++ptr_;
    return *this;
  }

  iterator operator+ (size_type n) {
    iterator tmp (ptr_ - n);
    return tmp;
  }

  iterator operator- (size_type n) {
    iterator tmp (ptr_ + n);
    return tmp;
  }

  iterator operator+(const iterator& other) {
     iterator tmp(ptr_ - other.ptr_);
     return tmp;
  }

  iterator operator-(const iterator& other) {
     iterator tmp(ptr_ + other.ptr_);
     return tmp;
  }

  iterator& operator+= (size_type n) {
    ptr_ -= n;
    return *this;
  }

  iterator& operator-= (size_type n) {
    ptr_ += n;
    return *this;
  }

  reference operator[](size_t count) {
     return ptr_[count];
  }
};

template<class T, class Alloc = Allocator<T>>
class Vector {
  T *data = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
  Alloc allocator;
public:
  using size_type = size_t;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using allocator_type = Alloc;
  using iterator = Iterator<T>;
  using reverce_iterator = ReverseIterator<T>;

  explicit Vector(size_type count = 0) {
    data = allocator.allocate (count);
    for (int i = 0; i < count; ++i)
      allocator.constr (data + i);
    size_ = count;
    capacity_ = count;
  }

  ~Vector () {
    clear ();
    allocator.deallocate (data);
  }

  bool empty () const {
    return size_ == 0;
  }

  size_type size () const {
    return size_;
  }

  size_type capacity () const {
    return capacity_;
  }

  const_reference operator[] (size_type n) const {
    return data[n];
  }

  reference operator[] (size_type n) {
    return data[n];
  }

  void pop_back () {
    --size_;
    allocator.destr(data + size_);
  }

  void push_back (const value_type& value) {
    if (size_ >= capacity_)
      reserve (capacity_ * 2 + 1);
    allocator.constr (data + size_, value);
    ++size_;
  }

  void push_back (value_type&& value) {
    if (size_ >= capacity_)
      reserve (capacity_ * 2 + 1);
    allocator.constr (data + size_, std::move (value));
    ++size_;
  }

  void resize (size_type new_size) {
    reserve (new_size);
    if (new_size > size_){
      for (size_type i = size_; i < new_size; ++i) {
        allocator.constr (data + i);
      }
    }
    else {
      for (size_type i = new_size; i < size_; ++i) {
        allocator.destr (data + i);
      }
    }
    size_ = new_size;
  }

  void reserve (size_type new_capacity) {
    if (capacity_ >= new_capacity)
      return;
    T *tmp = allocator.allocate (new_capacity);
    for (size_type i = 0; i < size_; ++i) {
      allocator.constr (tmp + i, data[i]);
      allocator.destr (data + i);
    }
    allocator.deallocate (data);
    data = tmp;
    capacity_ = new_capacity;
  }

  void clear () {
    for (size_type i = 0; i < size_; ++i)
      allocator.destr (data + i);
    size_ = 0;
  }

  reverce_iterator rbegin () {
    return reverce_iterator (data + size_ - 1);
  }

  reverce_iterator rend () {
    return reverce_iterator (data - 1);
  }

  iterator begin () {
    return iterator (data);
  }

  iterator end () {
    return iterator (data + size_);
  }

};
