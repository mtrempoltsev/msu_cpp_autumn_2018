#include <iostream>
#include <iterator>
#include <stdexcept>

template<class T>
class Allocator
{
    public:
        using Ttype = T;
        using pointer = T*;
        using size_type = size_t;

    public:
        template<class... ArgsT>
            void construct(pointer ptr, ArgsT&& ... args) {
                new (ptr) Ttype(std::forward<ArgsT>(args)...);
            }
        void kill(pointer ptr) {
            ptr->~Ttype();
        }
        pointer allocate(size_type size) {
            return static_cast<pointer>
                (::operator new[](sizeof(Ttype) * size));
        }
        void deallocate(pointer ptr, size_type size) {
            ::operator delete[](ptr, size * sizeof(Ttype));
        }
};


template<class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
    public:
        using Ttype = T;
        using reference = T&;
        using pointer = T*;
        using iterator = Iterator<Ttype>;
        using size_type = size_t;

    private:
        pointer _ptr_;

    public:
        Iterator (T *ptr): _ptr_(ptr) {}

        reference operator * () const {
            return *_ptr_;
        }

        iterator& operator [] (size_type n) {
            return _ptr_[n];
        }

        bool operator == (const Iterator<Ttype>& other) const {
            return _ptr_ == other._ptr_;
        }
        bool operator != (const Iterator<Ttype>& other) const {
            return _ptr_ != other._ptr_;
        }
        bool operator < (const Iterator<Ttype>& other) const {
            return _ptr_ < other._ptr_;
        }
        bool operator > (const Iterator<Ttype>& other) const {
            return _ptr_ > other._ptr_;
        }
        bool operator <= (const Iterator<Ttype>& other) const {
            return _ptr_ <= other._ptr_;
        }
        bool operator >= (const Iterator<Ttype>& other) const {
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

        iterator operator + (const iterator& other) {
            iterator t (_ptr_ + other._ptr_);
            return t;
        }
        iterator operator - (const iterator& other) {
            iterator t(_ptr_ - other._ptr_);
            return t;
        }

        iterator operator + (size_type n) {
            iterator t (_ptr_ + n);
            return t;
        }
        iterator operator - (size_type n) {
            iterator t (_ptr_ - n);
            return t;
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
        using Ttype = T;
        using pointer = T*;
        using const_reference = const T&;
        using reference = T&;
        using allocator_type = Alloc;
        using iterator = Iterator<T>;
        using reverse_iterator = std::reverse_iterator<iterator>;

    private:
        size_type alloc_size;
        pointer data;
        size_type _size_;

        Alloc alloc_;
    public:
        Vector(size_type size = 0):
            alloc_(),
            alloc_size(256),
            data(alloc_.allocate(alloc_size)),
            _size_(size)
        {
            for (pointer ptr = data; ptr != data + _size_; ++ptr) {
                alloc_.construct(ptr);
            }
        }

        ~Vector() {
            for (pointer ptr = data; ptr != data + _size_; ++ptr) {
                alloc_.kill(ptr);
            }
            alloc_.deallocate(data, alloc_size);
        }

        size_type size() const {
            return _size_;
        }

        size_type capacity() const {
            return alloc_size;
        }

        bool empty() const {
            return (_size_ == 0);
        }

        const_reference operator[](size_type pos) const {
            if (pos < _size_) {
                return data[pos];
            }
        }
        reference operator[](size_type pos) {
            if (pos < _size_) {
                return data[pos];
            }
        }

        void push_back(const_reference value) {
            if (_size_ >= alloc_size) {
                reserve(alloc_size * 2);
            }
            alloc_.construct(data + (_size_)++, value);
        }
        void push_back(Ttype&& value) {
            if (_size_ >= alloc_size) {
                reserve(alloc_size * 2);
            }
            alloc_.construct(data + (_size_)++, std::move(value));
        }

        void pop_back() {
            if (_size_ > 0) {
                --_size_;
                alloc_.kill(data + _size_);
            }
        }
        void clear() {
            for (auto ptr = data; ptr != data + _size_; ++ptr) {
                alloc_.kill(ptr);
            }
            _size_ = 0;
        }

        void reserve(size_type newsize) {
            if (newsize <= alloc_size) {
                return;
            }
            pointer tmp_data = alloc_.allocate(newsize);
            for (size_type i = 0; i < _size_; ++i) {
                alloc_.construct(tmp_data + i, std::move(*(data + i)));
                alloc_.kill(data + i);
            }
            alloc_.deallocate(data, alloc_size);
            data = tmp_data;
            alloc_size = newsize;
        }
        void resize(size_type newsize) {
            if (newsize < _size_) {
                for (pointer ptr = data + newsize; ptr != data + _size_; ++ptr) {
                    alloc_.kill(ptr);
                }
                _size_ = newsize;
            } else {
                if (newsize > alloc_size) {
                    this->reserve(newsize);
                }
                for (pointer ptr = data + _size_; ptr != data + newsize; ++ptr) {
                    alloc_.construct(ptr);
                }
                _size_ = newsize;
            }
        }

        iterator begin() {
            return iterator(data);
        }
        iterator end() {
            return iterator(data + _size_);
        }
        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }
        reverse_iterator rend() {
            return reverse_iterator(begin());
        }
};
