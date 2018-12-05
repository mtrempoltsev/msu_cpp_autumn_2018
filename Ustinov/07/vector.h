#include <iostream>
#include <stdexcept>


template <class T>
class Allocator {
 public:
  using value_type = T;
  using pointer = T*;

  pointer allocate(const size_t count) {
      pointer ptr = (pointer)malloc(sizeof(value_type) * count);
      return ptr;
  }

  void deallocate(pointer ptr) {
      free(ptr);
  }

  template <class... Args>
  void construct(pointer ptr, Args&&... arg)  {
      ptr = new(ptr) value_type(std::forward<Args>(arg)...);
  }

  void destroy(pointer ptr) {
      ptr->~value_type();
  }
};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T> {
   T *ptr_;
 public:
   using iterator = Iterator<T>;
   using value_type = T;
   using pointer = T*;
   using reference = T&;

   Iterator(T* ptr):
            ptr_(ptr)
            {}

   Iterator(const iterator& it):
            ptr_(it.ptr_)
            {}

   bool operator==(const iterator& other) {
      return ptr_ == other.ptr_;
   }

   bool operator!=(const iterator& other) {
      return !(*this == other);
   }

   bool operator<(const iterator& other) {
      return ptr_ < other.ptr_;
   }

   bool operator>(const iterator& other) {
      return ptr_ > other.ptr_;
   }

   reference operator*() {
      return *ptr_;
   }

   iterator& operator++() {
      ++ptr_;
      return *this;
   }

   iterator& operator--() {
      --ptr_;
      return *this;
   }

   iterator& operator+=(size_t count) {
      ptr_ += count;
      return *this;
   }

   iterator& operator-=(size_t count) {
      ptr_ -= count;
      return *this;
   }

   iterator operator+(size_t count) {
      iterator tmp(ptr_ + count);
      return tmp;
   }

   iterator operator-(size_t count) {
      iterator tmp(ptr_ - count);
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

   friend iterator operator+(size_t count, const iterator& other) {
     iterator tmp(other.ptr_ + count);
     return tmp;
   }

   friend iterator operator-(size_t count, const iterator& other) {
     iterator tmp(count - other.ptr_);
     return tmp;
   }

   reference operator[](size_t count) {
      return ptr_[count];
   }
};

template <class T, class Alloc = Allocator<T>>
class Vector {
 private:
   T *data_;
   size_t size_;
   size_t capacity_;
   Alloc alloc_;

 public:
   using iterator = Iterator<T>;
   using const_iterator = const Iterator<T>;
   using reverse_iterator = std::reverse_iterator<iterator>;
   using const_reverse_iterator = std::reverse_iterator<const_iterator>;
   using value_type = T;
   using reference = T&;
   using pointer = T*;

   Vector(const size_t count=0):
          data_(alloc_.allocate(count * 2)),
          capacity_(count * 2) {
              resize(count);
          }

   ~Vector() {
      clear();
      alloc_.deallocate(data_);
   }

   size_t size()  const {
      return size_;
   }

   bool empty() const {
      return (size_ == 0);
   }

   size_t capacity() const {
      return capacity_;
   }

   void push_back(const value_type& value) {
      if (capacity_ == size_) {
          reserve(capacity_ * 2 + 1);
      }
      alloc_.construct(data_ + size_, value);
      ++size_;
   }

   void pop_back() {
     if (size_ > 0) {
        alloc_.destroy(data_ + size_ - 1);
        --size_;
     }
   }

   void reserve(const size_t count) {
      if (count > capacity_) {
          pointer new_data = alloc_.allocate(count);
          for (int i=0; i < size_; ++i) {
              alloc_.construct(new_data + i, data_[i]);
          }
          for (int i=0; i<size_; ++i) {
              alloc_.destroy(data_ + i);
          }
          alloc_.deallocate(data_);
          data_ = new_data;
          capacity_ = count;
      }
   }

   void resize(const size_t count) {
      if (count > size_) {
          if (capacity_ > count) {
              for (int i=size_; i < count; ++i) {
                  alloc_.construct(data_ + i);
              }
          } else {
              reserve(count * 2 + 1);
              for (int i=size_; i < (count - size_); ++i) {
                  alloc_.construct(data_ + i);
              }
          }
      } else if (count < size_) {
          for (int i=count; i < size_; ++i) {
              alloc_.destroy(data_ + i);
          }
      }
      size_ = count;
   }

   void clear() {
      resize(0);
   }

   reference operator[](size_t pos) {
      if (pos >= size_)
          throw std::out_of_range("out");
      return data_[pos];
   }
   const reference operator[](size_t pos) const {
      if (pos >= size_)
          throw std::out_of_range("out");
      return data_[pos];
   }

   iterator begin() {
      return iterator(data_);
   }

   iterator end() {
      return iterator(data_ + size_);
   }

   reverse_iterator rbegin() {
      return reverse_iterator(data_ + size_);
   }

   reverse_iterator rend() {
      return reverse_iterator(data_);
   }
};
