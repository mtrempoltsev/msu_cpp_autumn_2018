#include <iterator>
#include <stdexcept>

template<class T>
class Allocator
{
public:
    
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    template<class... ArgsT>
    void construct(pointer ptr, ArgsT&& ... Args);
    pointer allocate(size_type size);
    void deallocate(pointer ptr, size_type size);
    void destroy(pointer ptr);
};


template<class T>
template<class... ArgsT>
void Allocator<T>::construct(pointer ptr, ArgsT&& ... Args)
{
    new (ptr)T(std::forward<ArgsT>(Args)...);
}

template<class T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type size)
{
    return static_cast<Allocator<T>::pointer>
        (::operator new[](sizeof(T) * size));
}

template<class T>
void Allocator<T>::deallocate(pointer ptr, size_type size)
{
    ::operator delete[](ptr);
}


template<class T>
void Allocator<T>::destroy(pointer ptr)
{
    ptr->~T();
}


template<class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
    T* ptr_;

public:
    using ref = T&;
    explicit Iterator(T* ptr) noexcept: ptr_{ptr} {};
    ref operator*() const noexcept;
    bool operator==(const Iterator<T>& other) const noexcept;
    bool operator!=(const Iterator<T>& other) const noexcept;
    Iterator& operator++() noexcept;
    Iterator& operator--() noexcept;
};


template<class T>
typename Iterator<T>::ref Iterator<T>::operator*() const noexcept
{
    return *ptr_;
}


template<class T>
bool Iterator<T>::operator==(const Iterator<T>& other) const noexcept
{
    return ptr_ == other.ptr_;
}

template<class T>
bool Iterator<T>::operator!=(const Iterator<T>& other) const noexcept
{
    return ptr_ != other.ptr_;
}

template<class T>
Iterator<T>& Iterator<T>::operator++() noexcept
{
    ++ptr_;
    return *this;
}

template<class T>
Iterator<T>& Iterator<T>::operator--() noexcept
{
    --ptr_;
    return *this;
}


template<class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using const_ref = const T&;
    using ref = T&;
    using allocator_type = Alloc;
    using iterator = Iterator<T>;

private:
    Alloc alloc_;
    size_type allocated_;
    T* data_;
    size_type used_;
    static constexpr size_type base_alloc_{16};

public:
    Vector(size_type size = 0): alloc_(), 
                                allocated_(base_alloc_),
                                data_(alloc_.allocate(allocated_)),
                                used_(size) 
    {
        for (pointer ptr = data_; ptr != data_ + used_; ++ptr) {
            alloc_.construct(ptr);
        }
    }

    ~Vector()
    {
        for (pointer ptr = data_; ptr != data_ + used_; ++ptr) {
            alloc_.destroy(ptr);
        }
        alloc_.deallocate(data_, allocated_);
    }

    void reserve(size_type size);
    void push_back(const_ref value);
    void push_back(value_type&& value);
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    void resize(size_type size);
    const_ref operator[](size_type pos) const;
    ref operator[](size_type pos);
    void pop_back();
    void clear();
    bool empty() const noexcept;
    iterator begin() noexcept;
    std::reverse_iterator<iterator> rbegin();
    iterator end() noexcept;
    std::reverse_iterator<iterator> rend();
};


template<class T, class Alloc>
void Vector<T, Alloc>::reserve(size_type size)
{
    if (size <= allocated_) 
        return;
    pointer new_data = alloc_.allocate(size);

    for (size_type ind = 0; ind < used_; ++ind) {
        alloc_.construct(new_data + ind, 
                         std::move(*(data_ + ind)));
        alloc_.destroy(data_ + ind);
    }
    alloc_.deallocate(data_, allocated_);
    data_ = new_data;
    allocated_ = size;
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(value_type&& value)
{
    if (used_ >= allocated_) {
        reserve(allocated_ * 2);
    }
    alloc_.construct(data_ + (used_)++, std::move(value));
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const_ref value)
{
    if (used_ >= allocated_) {
        reserve(allocated_ * 2);
    }
    alloc_.construct(data_ + (used_)++, value);
}

template<class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::size() const noexcept
{
    return used_;
}

template<class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::capacity() const noexcept
{
    return allocated_;
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(size_type size)
{
    if (size < used_) {
        for (pointer ptr = data_ + size; ptr != data_ + used_; ++ptr) {
            alloc_.destroy(ptr);
        }
        used_ = size;
        return;
    }
    if (size < allocated_) {
        for (auto ptr = data_ + used_; ptr != data_ + size; ++ptr) {
            alloc_.construct(ptr);
        }
        used_ = size;
        return;
    } else {
        this->reserve(size);
        for (auto ptr = data_ + used_; ptr != data_ + size; ++ptr) {
            alloc_.construct(ptr);
        }
        used_ = size;
    }
}

template<class T, class Alloc>
typename Vector<T, Alloc>::const_ref 
    Vector<T, Alloc>::operator[](size_type pos) const
{
    if (pos < used_) {
        return data_[pos];
    } else {
        throw std::out_of_range("Wrong index");
    }
}

template<class T, class Alloc>
typename Vector<T, Alloc>::ref Vector<T, Alloc>::operator[](
    size_type pos)
{
    if (pos < used_) {
        return data_[pos];
    } else {
        throw std::out_of_range("Wrong index");
    }
}

template<class T, class Alloc>
void Vector<T, Alloc>::pop_back()
{
    if (used_ > 0) {
        --used_;
        alloc_.destroy(data_ + used_);
    } else {
        throw std::out_of_range("Wrong pop back");
    }
}

template<class T, class Alloc>
void Vector<T, Alloc>::clear()
{
    for (auto ptr = data_; ptr != data_ + used_; ++ptr) {
        alloc_.destroy(ptr);
    }
    used_ = 0;
}

template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const noexcept
{
    return (used_ == 0);
}

template<class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::begin() noexcept
{
    return iterator(data_);
}

template<class T, class Alloc>
std::reverse_iterator<typename Vector<T, Alloc>::iterator>
Vector<T, Alloc>::rbegin()
{
    return std::reverse_iterator<iterator>(iterator(data_ + used_));
}

template<class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::end() noexcept
{
    return iterator(data_ + used_);
}

template<class T, class Alloc>
std::reverse_iterator<typename Vector<T, Alloc>::iterator>
Vector<T, Alloc>::rend()
{
    return std::reverse_iterator<iterator>(iterator(data_));
}
