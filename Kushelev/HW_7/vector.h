#include <iterator>

template<class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type n)
    {
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type n)
    {
        ::operator delete(p, n * sizeof(value_type));
    }

    template <class... Args>
    void construct(pointer p, Args&&... args)
    {
        ::new(p) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer p)
    {
        p->~value_type();
    }
};

template<class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator = Iterator<value_type>;

    explicit Iterator(pointer p) : ptr_(p)
    {}

    Iterator(const iterator& it) : ptr_(it.ptr_)
    {}

    bool operator==(const iterator& other)
    {
        return (ptr_ == other.ptr_);
    }

    bool operator!=(const iterator& other)
    {
        return !(*this == other);
    }
	
    bool operator<(const iterator& other)
    {
	return (ptr_ < other.ptr_);
    }

    bool operator>(const iterator& other)
    {
	return other < *this;
    }

    bool operator>=(const iterator& other)
    {
	return !(*this < other);
    }

    bool operator<=(const iterator& other)
    {
	return !(*this > other);
    }

    reference operator*()
    {
        return *ptr_;
    }

    iterator& operator++()
    {
        ptr_++;
        return *this;
    }

    iterator& operator--()
    {
        ptr_--;
        return *this;
    }

    reference operator[](size_t n)
    {
        return ptr_[n];
    }

    iterator& operator+=(size_t n)
    {
	ptr_ += n;
	return *this;
    }

    iterator& operator-=(size_t n)
    {
	ptr_ -= n;
	return *this;
    }

    iterator operator+(size_t n)
    {
	iterator tmp = *this;
	return tmp += n;
    }

    iterator operator+(const iterator& oth)
    {
        iterator tmp(ptr_ + oth.ptr_);
        return tmp;
    }

    iterator operator-(const iterator& oth)
    {
        iterator tmp(ptr_ - oth.ptr_);
        return tmp;
    }

    iterator operator-(size_t n)
    {
	iterator tmp = *this;
	return tmp -= n;
    }

    private:
    pointer ptr_;
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;
    using size_type = size_t;
    using iterator = Iterator<value_type>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    explicit Vector ()
        : size_(0), alloc_(), capacity_(8), data_(alloc_.allocate(8))
        {};

    explicit Vector(size_type n)
        : size_(n), alloc_(), capacity_(2 * n), data_(alloc_.allocate(2 * n))
    {
        for (pointer ptr = data_; ptr < data_ + size_; ptr++)
            alloc_.construct(ptr);
    }

    ~Vector()
    {
        clear();
        alloc_.deallocate(data_, capacity_);
    }

    reference operator[] (size_type i)
    {
        if (i < size_)
            return data_[i];
    }

    const_reference operator[] (size_type i) const
    {
        if (i < size_)
            return data_[i];
    }

    void reserve(const size_type new_cap)
    {
        if (new_cap <= size_)
            return;

        pointer new_data_ = alloc_.allocate(new_cap);

        for (size_type i = 0; i < size_; i++)
        {
            alloc_.construct(new_data_ + i, data_[i]);
            alloc_.destroy(data_ + i);
        }

        alloc_.deallocate(data_, capacity_);

        data_ = new_data_;
        capacity_ = new_cap;
    }

    void resize(const size_type new_size)
    {
        if (new_size < size_)
        {
            for (size_type i = new_size; i < size_; i++)
            {
                alloc_.destroy(data_ + i);
            }

            size_ = new_size;
            return;
        }

        if(new_size >= capacity_)
            reserve(new_size);

        for (size_type i = size_; i < new_size; i++)
        {
            alloc_.construct(data_ + i);
        }

        size_ = new_size;
        return;
    }

    void clear()
    {
        while (size_ > 0)
        {
            alloc_.destroy(data_ + size_);
            size_--;
        }
    }

    void push_back(value_type&& value)
    {
        if (size_ == capacity_)
            reserve(2 * capacity_);
        alloc_.construct(data_ + size_, std::move(value));
        size_++;
    }

    void pop_back()
    {
        if (size_ > 0)
        {
            size_--;
            alloc_.destroy(data_ + size_);
        }
    }

    iterator begin()
    {
        return iterator(data_);
    }

    iterator end()
    {
        return iterator(data_ + size_);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    size_type size() const
    {
        return size_;
    }

    size_type capacity()
    {
        return capacity_;
    }

    bool empty() const
    {
        return (size_ == 0);
    }

    private:
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type capacity_;
};
