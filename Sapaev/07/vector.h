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
    void construct(pointer ptr, ArgsT&& ... args)
    {
        new (ptr) arg_type(std::forward<ArgsT>(args)...);
    }
    void destroy(pointer ptr)
    {
        ptr->~T();
    }
    pointer allocate(size_type size)
    {
        return static_cast<pointer>
        (::operator new[](sizeof(arg_type) * size));
    }
    void deallocate(pointer ptr, size_type size)
    {
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
    
private:
    pointer _ptr_;
    
public:
    explicit Iterator(pointer ptr) noexcept: _ptr_{ptr} {};
    
    reference operator * () const noexcept
    {
        return *_ptr_;
    }
    
    iterator& operator [] (int n)
    {
        return _ptr_[n];
    }

    bool operator == (const Iterator<arg_type>& other) const noexcept
    {
        return _ptr_ == other._ptr_;
    }
    bool operator != (const Iterator<arg_type>& other) const noexcept
    {
        return _ptr_ != other._ptr_;
    }
    bool operator < (const Iterator<arg_type>& other) const noexcept
    {
        return _ptr_ < other._ptr_;
    }
    bool operator > (const Iterator<arg_type>& other) const noexcept
    {
        return _ptr_ > other._ptr_;
    }
    bool operator <= (const Iterator<arg_type>& other) const noexcept
    {
        return _ptr_ <= other._ptr_;
    }
    bool operator >= (const Iterator<arg_type>& other) const noexcept
    {
        return _ptr_ >= other._ptr_;
    }
    
    iterator& operator ++ () noexcept
    {
        ++_ptr_;
        return *this;
    }
    iterator& operator -- () noexcept
    {
        --_ptr_;
        return *this;
    }
    
    iterator operator ++ (int) noexcept
    {
        auto tmp = this;
        ++_ptr_;
        return *tmp;
    }
    iterator operator -- (int) noexcept
    {
        auto tmp = this;
        --_ptr_;
        return *tmp;
    }
    
    iterator operator + (const iterator& other) noexcept
    {
        auto tmp = this;
        tmp._ptr_ += other._ptr_;
        return *tmp;
    }
    iterator operator - (const iterator& other) noexcept
    {
        auto tmp = this;
        tmp._ptr_ -= other._ptr_;
        return *tmp;
    }
    
    iterator operator + (int n) noexcept
    {
        auto tmp = this;
        _ptr_ += n;
        return *tmp;
    }
    iterator operator - (int n) noexcept
    {
        auto tmp = this;
        _ptr_ -= n;
        return *tmp;
    }
    
    iterator& operator += (int n) noexcept
    {
        _ptr_ += n;
        return *this;
    }
    iterator& operator -= (int n) noexcept
    {
        _ptr_ -= n;
        return *this;
    }
    
    friend iterator& operator + (int n, const iterator& it) noexcept
    {
        return n + it._ptr_;
    }
    friend iterator& operator - (int n, const iterator& it) noexcept
    {
        return n - it._ptr_;
    }
};


template<class T, class Alloc = Allocator<T>>
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
    static constexpr size_type _base_size_{256};
    Alloc _alloc_;
    size_type _allocated_;
    pointer _data_;
    size_type _size_;
    
public:
    Vector(size_type size = 0):
        _alloc_(),
        _allocated_(_base_size_),
        _data_(_alloc_.allocate(_allocated_)),
        _size_(size)
    {
        for (pointer ptr = _data_; ptr != _data_ + _size_; ++ptr) {
            _alloc_.construct(ptr);
        }
    }
    
    ~Vector()
    {
        for (pointer ptr = _data_; ptr != _data_ + _size_; ++ptr) {
            _alloc_.destroy(ptr);
        }
        _alloc_.deallocate(_data_, _allocated_);
    }
    
    size_type size() const noexcept
    {
        return _size_;
    }
    size_type capacity() const noexcept
    {
        return _allocated_;
    }
    
    bool empty() const noexcept
    {
        return (_size_ == 0);
    }

    const_reference operator[](size_type pos) const
    {
        if (pos < _size_) {
            return _data_[pos];
        } else {
            throw std::out_of_range("Index out of range in operator[]");
        }
    }
    reference operator[](size_type pos)
    {
        if (pos < _size_) {
            return _data_[pos];
        } else {
            throw std::out_of_range("Index out of range in operator[]");
        }
    }

    void push_back(const_reference value)
    {
        if (_size_ >= _allocated_) {
            reserve(_allocated_ * 2);
        }
        _alloc_.construct(_data_ + (_size_)++, value);
    }
    void push_back(arg_type&& value)
    {
        if (_size_ >= _allocated_) {
            reserve(_allocated_ * 2);
        }
        _alloc_.construct(_data_ + (_size_)++, std::move(value));
    }
    
    void pop_back()
    {
        if (_size_ > 0) {
            --_size_;
            _alloc_.destroy(_data_ + _size_);
        } else {
            throw std::out_of_range("Empty vector, can't pop_back");
        }
    }
    
    void clear()
    {
        for (auto ptr = _data_; ptr != _data_ + _size_; ++ptr) {
            _alloc_.destroy(ptr);
        }
        _size_ = 0;
    }
    
    void reserve(size_type size)
    {
        if (size <= _allocated_) {
            return;
        }
        pointer tmp_data = _alloc_.allocate(size);
        for (size_type i = 0; i < _size_; ++i) {
            _alloc_.construct(tmp_data + i, std::move(*(_data_ + i)));
            _alloc_.destroy(_data_ + i);
        }
        _alloc_.deallocate(_data_, _allocated_);
        _data_ = tmp_data;
        _allocated_ = size;
    }
    
    void resize(size_type size)
    {
        if (size < _size_) {
            for (pointer ptr = _data_ + size; ptr != _data_ + _size_; ++ptr) {
                _alloc_.destroy(ptr);
            }
            _size_ = size;
        } else {
            if (size > _allocated_) {
                this->reserve(size);
            }
            for (pointer ptr = _data_ + _size_; ptr != _data_ + size; ++ptr) {
                _alloc_.construct(ptr);
            }
            _size_ = size;
        }
    }
    
    iterator begin()
    {
        return iterator(_data_);
    }
    iterator end()
    {
        return iterator(_data_ + _size_);
    }
    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }
    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }
};
