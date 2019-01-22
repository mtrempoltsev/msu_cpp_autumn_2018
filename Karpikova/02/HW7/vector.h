using namespace std;

template<class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type Num)
    {
        void *ptr_ = nullptr;

        if (Num && !(ptr_ = ::operator new(Num * sizeof(value_type)))) {
            throw bad_alloc();
        }
        return static_cast<pointer>(ptr_);
    }

    void deallocate(pointer ptr_)
    {
        ::operator delete(ptr_);
    }

    template <typename... ArgsT>
    void construct(pointer ptr_, ArgsT&&... args)
    {
        ::new(ptr_) T(std::forward <ArgsT> (args)...);
    }

    void destroy(pointer ptr_) {
        ptr_->~value_type();
    }
};

template <class T>
class Iterator : public std::iterator<std::forward_iterator_tag, T>
{
    T* ptr_;
    bool frw;
public:
    using reference = T&;

    explicit Iterator(T* ptr, bool frw = true)
        : ptr_(ptr), frw(frw)
    {
    }

    bool operator==(const Iterator<T>& other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const
    {
        return !(*this == other);
    }

    reference operator*() const
    {
        return *ptr_;
    }

    Iterator& operator++()
    {
        frw ? ++ptr_ : --ptr_;
        return *this;
    }
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

private:
    size_type size_;
    size_type capacity_;
    pointer first;
    Alloc alloc_;

public:

    Vector() : size_(0), capacity_(0), first(nullptr)
    {
    }

    Vector(size_type count)
        : size_(count), first(alloc_.allocate(count * 2)),
          capacity_(count * 2)
    {
        for (size_type i = 0; i < size_; ++i) {
            alloc_.construct(first + i);
        }
    }

    ~Vector()
    {
        for (size_type i = 0; i < size_; ++i) {
	    alloc_.destroy(first + i);
	}
        alloc_.deallocate(first);
    }

    size_type size() const
    {
        return size_;
    }

    size_type capacity() const
    {
        return capacity_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    const_reference operator[](size_type pos) const
    {
        if (pos >= size_)
            throw std::out_of_range("Out of range");
        return *(first + pos);
    }

    iterator begin() noexcept
    {
        return iterator(first);
    }

    iterator rbegin() noexcept
    {
        return iterator(first + size_ - 1, false);
    }

    iterator end() noexcept
    {
        return iterator(first + size_);
    }

    iterator rend() noexcept
    {
        return iterator(first - 1, false);
    }

    void push_back(value_type&& value)
    {
        if (size_ == capacity_) {
            size_type newCap = capacity_ * 2 + 1;
            reserve(newCap);
            capacity_ = newCap;
        }
        alloc_.construct(first + size_, value);
        ++size_;
    }

    void push_back(const value_type& value)
    {
        if (size_ == capacity_) {
            size_type newCap = capacity_ * 2 + 1;
            reserve(newCap);
            capacity_ = newCap;
        }
        alloc_.construct(first + size_, value);
        ++size_;
    }

    void pop_back()
    {
        if (size_ > 0) {
            alloc_.destroy(first + size_);
            --size_;
        }
    }

    void reserve(size_type Num) // Выделяет память
    {
        if (Num > capacity_) {
            capacity_ = Num;
            pointer newFirst = alloc_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i) {
                alloc_.construct(newFirst + i, *(first + i));
                alloc_.destroy(first + i);
            }
            alloc_.deallocate(first);
            first = newFirst;
        }
    }

    void resize(size_type newSize) // Изменяет размер
    {
        if (newSize > size_) {
            reserve(newSize);
        }
        for (size_type i = newSize; i < size_; ++i) {
                alloc_.destroy(first + i);
        }
        for (size_type i = size_; i < newSize; ++i) {
                alloc_.construct(first + i);
        }
        size_ = newSize;
    }

    void clear() noexcept
    {
        resize(0);
    }
};