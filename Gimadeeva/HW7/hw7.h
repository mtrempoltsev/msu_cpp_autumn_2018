#include <iostream>
#include <iterator>
#include <stdexcept>

template<class T>
class Allocator
{
    public:
        using arg_type = T;
        using pointer = T*;
        using size_type = size_t;

    public:
        template<class... ArgsT>
        void construct(pointer ptr, ArgsT&& ... args) {
            new(ptr) arg_type(std::forward<ArgsT>(args)...);
        }

        void destroy(pointer ptr) {
            ptr->~arg_type();
        }
        pointer allocate(size_type size) {
            return static_cast<pointer>
                (::operator new[](sizeof(arg_type) * size));
        }
        void deallocate(pointer ptr, size_type size) {
            ::operator delete[](ptr, size * sizeof(arg_type));
        }
};


template<class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
    public:
        using arg_type = T;
        using reference = T&;
        using pointer = T*;
        using iterator = Iterator<arg_type>;
        using size_type = size_t;

    private:
        pointer _ptr_;

    public:
        Iterator (T *ptr): _ptr_(ptr) {}

        reference operator * () const {
            return *_ptr_;
        }

        reference operator [] (size_type n) {
            return _ptr_[n];
        }

        bool operator == (const Iterator<arg_type>& other) const {
            return _ptr_ == other._ptr_;
        }
        bool operator != (const Iterator<arg_type>& other) const {
            return _ptr_ != other._ptr_;
        }
        bool operator < (const Iterator<arg_type>& other) const {
            return _ptr_ < other._ptr_;
        }
        bool operator > (const Iterator<arg_type>& other) const {
            return _ptr_ > other._ptr_;
        }
        bool operator <= (const Iterator<arg_type>& other) const {
            return _ptr_ <= other._ptr_;
        }
        bool operator >= (const Iterator<arg_type>& other) const {
            return _ptr_ >= other._ptr_;
        }

        iterator& operator ++ () {
            ++_ptr_;
            return *this;
        }
        iterator& operator -- () {
            --_ptr_;
            return *this;
        }

        size_type operator - (const iterator& other) const {
            return this._ptr_ - other._ptr_;
        }

        iterator operator + (size_type n) const {
            return iterator(_ptr_ + n);
        }
        iterator operator - (size_type n) const {
            return iterator(_ptr_ - n);
        }

        iterator& operator += (size_type n) {
            _ptr_ += n;
            return *this;
        }
        iterator& operator -= (size_type n) {
            _ptr_ -= n;
            return *this;
        }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
    public:
        using size_type = size_t;
        using arg_type = T;
        using pointer = T*;
        using const_reference = const T&;
        using reference = T&;
        using allocator_type = Alloc;
        using iterator = Iterator<T>;
        using reverse_iterator = std::reverse_iterator<iterator>;

    private:
        Alloc alloc_;
        size_type allocated_;
        pointer data_;
        size_type size_;

    public:
        template <class... Args>
        explicit Vector(size_type size = 0, Args&&... args):
            alloc_(),
            allocated_(size),
            data_(alloc_.allocate(allocated_)),
            size_(size)
        {
            for (pointer ptr = data_; ptr != data_ + size_; ++ptr) {
                alloc_.construct(ptr, args...);
            }
        }

        ~Vector() {
            for (pointer ptr = data_; ptr != data_ + size_; ++ptr) {
                alloc_.destroy(ptr);
            }
            alloc_.deallocate(data_, allocated_);
        }

        size_type size() const {
            return size_;
        }

        size_type capacity() const {
            return allocated_;
        }

        bool empty() const {
            return (size_ == 0);
        }

        const_reference operator[](size_type pos) const {
            if (pos >= size_) {
                throw std::out_of_range("");
            }
            return data_[pos];
        }
        reference operator[](size_type pos) {
            if (pos >= size_) {
                throw std::out_of_range("");
            }
            return data_[pos];
        }

        void push_back(const_reference value) {
            if (size_ >= allocated_) {
                reserve(std::max<size_type>(allocated_ * 2, 1));
            }
            alloc_.construct(data_ + (size_)++, value);
        }
        void push_back(arg_type&& value) {
            if (size_ >= allocated_) {
                reserve(std::max<size_type>(allocated_ * 2, 1));
            }
            alloc_.construct(data_ + (size_)++, std::move(value));
        }

        void pop_back() {
            if (size_ > 0) {
                --size_;
                alloc_.destroy(data_ + size_);
            }
        }
        void clear() {
            for (auto ptr = data_; ptr != data_ + size_; ++ptr) {
                alloc_.destroy(ptr);
            }
            size_ = 0;
        }

        void reserve(size_type newsize) {
            if (newsize <= allocated_) {
                return;
            }
            pointer tmp_data = alloc_.allocate(newsize);
            std::copy(data_, data_ + size_, tmp_data);
            for (size_type i = 0; i < size_; ++i) {
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_, allocated_);
            data_ = tmp_data;
            allocated_ = newsize;
        }
        void resize(size_type newsize) {
            if (newsize < size_) {
                for (pointer ptr = data_ + newsize; ptr != data_ + size_; ++ptr) {
                    alloc_.destroy(ptr);
                }
            } else {
                if (newsize > allocated_) {
                    this->reserve(newsize);
                }
                for (pointer ptr = data_ + size_; ptr != data_ + newsize; ++ptr) {
                    alloc_.construct(ptr);
                }
            }
            size_ = newsize;
        }

        iterator begin() {
            return iterator(data_);
        }
        iterator end() {
            return iterator(data_ + size_);
        }
        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }
        reverse_iterator rend() {
            return reverse_iterator(begin());
        }
};
