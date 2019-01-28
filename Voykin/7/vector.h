#include <exception>
#include <iterator>
using namespace std;

template<class T>
class Allocator
{
public:
	T* allocate(const int n_elem)
	{
		T* ptr = (T*)malloc(sizeof(T)*n_elem);
		return ptr;
	};
	template <class... Args>
	void construct(T* ptr, Args&&... arg)
	{
		ptr = new(ptr) T(forward<Args>(arg)...);
	};
	void destroy(T* ptr)
	{
		ptr->~T();
	};
	void deallocate(T* ptr)
	{
		free(ptr);
	};
};

template <class T>
class Iterator: public iterator<random_access_iterator_tag, T>
{
    T* position_;
public:
    Iterator(T* other)
    {
        position_ = other;
    }

    bool operator == (const Iterator<T>& other) const
    {
        return position_ == other.position_;
    }

    bool operator != (const Iterator<T>& other) const
    {
        return !(position_ == other.position_);
    }

    T& operator* ()
    {
        return *position_;
    }

    Iterator<T>& operator++ ()
    {
        position_++;
        return *this;
    }

    Iterator<T>& operator-- ()
    {
        position_--;
        return *this;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
    size_t size_;
    size_t capacity_;
    T* data_;
    Alloc alloc_;
public:
    Vector(): size_(0), capacity_(10), data_(alloc_.allocate(capacity_))
    {}

    ~Vector()
    {
        for (size_t i = 0; i < size_;i++) {
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_);
    }

    bool empty() const noexcept
    {
        return size_ == 0;
    }

    size_t size() const noexcept
    {
        return size_;
    }

    size_t capacity() const noexcept
    {
        return capacity_;
    }

    auto begin() const noexcept
    {
        return Iterator<T>(data_);
    }

    auto end() const noexcept
    {
        return Iterator<T>(data_ + size_);
    }

    auto rbegin() const noexcept
    {
        return reverse_iterator<Iterator<T>>(data_ + size_);
    }

    auto rend() const noexcept
    {
        return reverse_iterator<Iterator<T>>(data_);
    }

    void push_back(T&& item)
    {
        if (size_ == capacity_)
        {
            reserve(capacity_ * 2);
        }
        alloc_.construct(data_ + size_, move(item));
        size_++;
    }

    void pop_back()
    {
        if (size_ == 0)
        {
            throw out_of_range("");
        }
        alloc_.destroy(data_ + --size_);
    }

    void reserve(size_t n)
    {
        if (capacity_ < n)
        {
            T* tmp = alloc_.allocate(n);
            for (size_t i = 0; i < size_; ++i)
            {
                alloc_.construct(tmp + i, move(*(data_ + i)));
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_);
            data_ = tmp;
            capacity_ = n;
        }
    }

    void resize(size_t n)
    {
        if (size_ >= n)
        {
            for (size_t i = n; i < size_; i++)
            {
                alloc_.destroy(data_ + i);
            }
        }
        else
        {
            if (n > capacity_)
            {
                reserve(n);
            }
            for (size_t i = size_; i < n; i++)
            {
                alloc_.construct(data_ + i);
            }
        }
        size_ = n;
    }

    void clear()
    {
        for (size_t i = 0; i < size_; i++)
        {
            alloc_.destroy(data_ + i);
        }
        size_ = capacity_ = 0;
    }

    T& operator[](size_t n)
    {
        if (n > size_)
        {
            throw out_of_range("");
        }
        return data_[n];
    }

};
