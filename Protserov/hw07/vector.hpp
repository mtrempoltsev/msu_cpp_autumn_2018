#include <cstring>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <iterator>

template<class T>
class Allocator
{
    public:
        using value_type = T;
        using pointer = T*;
        using size_type = size_t;

        pointer allocate(size_type count);
        void deallocate(pointer ptr, size_type count);

        template<class... ArgsT>
        void construct(pointer ptr, ArgsT&& ... Args);

        void destroy(pointer ptr);
};

template<class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
        T* ptr_;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        using reference = T&;
        explicit Iterator(T* ptr) noexcept: ptr_{ptr} {};
        reference operator*() const noexcept;
        bool operator==(const Iterator<T>& other) const noexcept;
        bool operator!=(const Iterator<T>& other) const noexcept;
        Iterator& operator++() noexcept;
        Iterator& operator--() noexcept;
};

template<class T, class Alloc = Allocator<T>>
class Vector
{
    public:
        using size_type = size_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using const_reference = const T&;
        using allocator_type = Alloc;
        using iterator = Iterator<T>;

        explicit Vector(size_type count = 0): alloc_{}, allocd_{basealloc},
            data_{alloc_.allocate(allocd_)}, used_{count} {
                for (auto ptr = data_; ptr != data_ + used_; ++ptr) {
                    alloc_.construct(ptr);
                }
         };
        ~Vector();
        void reserve(size_type count);
        void push_back(value_type&& value);
        void push_back(const_reference value);
        size_type size() const noexcept;
        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;
        void pop_back();
        bool empty() const noexcept;
        void clear();
        iterator begin() noexcept;
        std::reverse_iterator<iterator> rbegin();
        iterator end() noexcept;
        std::reverse_iterator<iterator> rend();
        void resize(size_type count);
        size_type capacity() const noexcept;
    private:
        Alloc alloc_;
        size_type allocd_;
        T* data_;
        size_type used_;
        static constexpr size_type basealloc{20};
        static constexpr size_type multipl{2};
};

template<class T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type count)
{
    return static_cast<Allocator<T>::pointer>(::operator new[](sizeof(T) * count));
}

template<class T>
void Allocator<T>::deallocate(pointer ptr, size_type count)
{
    ::operator delete[](ptr, sizeof(T) * count);
}

template<class T>
template<class... ArgsT>
void Allocator<T>::construct(pointer ptr, ArgsT&& ... Args)
{
    new (ptr) T{std::forward<ArgsT>(Args)...};
}

template<class T>
void Allocator<T>::destroy(pointer ptr)
{
    ptr->~T();
}

template<class T>
typename Iterator<T>::reference Iterator<T>::operator*() const noexcept
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

template<class T, class Alloc>
Vector<T, Alloc>::~Vector<T, Alloc>()
{
    for (auto ptr = data_; ptr != data_ + used_; ++ptr) {
        alloc_.destroy(ptr);
    }
    alloc_.deallocate(data_, allocd_);
}

template<class T, class Alloc>
void Vector<T, Alloc>::reserve(size_type count)
{
    if (count <= allocd_) {
        return;
    }
    pointer newdata = alloc_.allocate(count);
    std::copy(data_, data_ + used_, newdata);
    for (auto ptr = data_; ptr != data_ + used_; ++ptr) {
        alloc_.destroy(ptr);
    }
    alloc_.deallocate(data_, allocd_);
    data_ = newdata;
    allocd_ = count;
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(value_type&& value)
{
    if (used_ >= allocd_) {
        reserve(allocd_ * multipl);
    }
    new (data_ + (used_)++) value_type{std::forward<value_type>(value)};
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const_reference value)
{
    if (used_ >= allocd_) {
        reserve(allocd_ * multipl);
    }
    new (data_ + (used_)++) value_type{value};
}

template<class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::size() const noexcept
{
    return used_;
}

template<class T, class Alloc>
typename Vector<T, Alloc>::reference Vector<T, Alloc>::operator[](
    size_type pos)
{
    if (pos < used_) {
        return data_[pos];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

template<class T, class Alloc>
typename Vector<T, Alloc>::const_reference Vector<T, Alloc>::operator[](
    size_type pos) const
{
    if (pos < used_) {
        return data_[pos];
    } else {
        throw std::out_of_range("Index out of range");
    }
}


template<class T, class Alloc>
void Vector<T, Alloc>::pop_back()
{
    if (used_ > 0) {
        --used_;
        alloc_.destroy(data_ + used_);
    } else {
        throw std::out_of_range("Attempt to pop_back from an empty vector");
    }
}

template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const noexcept
{
    return (used_ == 0);
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
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::begin() noexcept
{
    return iterator{data_};
}
template<class T, class Alloc>
std::reverse_iterator<typename Vector<T, Alloc>::iterator>
Vector<T, Alloc>::rbegin()
{
    return std::reverse_iterator<iterator> {iterator{data_ + used_}};
}

template<class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::end() noexcept
{
    return iterator{data_ + used_};
}

template<class T, class Alloc>
std::reverse_iterator<typename Vector<T, Alloc>::iterator>
Vector<T, Alloc>::rend()
{
    return std::reverse_iterator<iterator> {iterator{data_}};
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(size_type count)
{
    if (count < used_) {
        for (auto ptr = data_ + count; ptr != data_ + used_; ++ptr) {
            alloc_.destroy(ptr);
        }
        used_ = count;
        return;
    }
    if (count < allocd_) {
        for (auto ptr = data_ + used_; ptr != data_ + count; ++ptr) {
            alloc_.construct(ptr);
        }
        used_ = count;
        return;
    } else {
        this->reserve(count);
        for (auto ptr = data_ + used_; ptr != data_ + count; ++ptr) {
            alloc_.construct(ptr);
        }
        used_ = count;
    }
}

template<class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::capacity() const noexcept
{
    return allocd_;
}
