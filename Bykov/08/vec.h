template <class T>
class Allocator
{
public:
    T* allocate(size_t count)
    {
        return static_cast<T*>(malloc(count * sizeof(T)));
    }

    void deallocate(T* ptr, size_t count)
    {
        if(count > 0)
            free(ptr);
    }

    template <class... Args>
    void construct(T *ptr, Args&&... args)
    {
        ::new (ptr) T(std::forward<Args>(args)...);
    }

    void destroy(T* ptr)
    {
        ptr->~T();
    }

    size_t max_size() const noexcept
    {
        return std::numeric_limits<size_t>::max();
    }
};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
    T *ptr_;
    bool inv_dir;

public:
    Iterator(T* ptr, bool invdir=false)
        : ptr_(ptr), inv_dir(invdir)
    {
    }

    bool operator ==(const Iterator& other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator !=(const Iterator& other) const
    {
        return !(*this == other);
    }

    Iterator &operator ++()
    {
        ptr_ += inv_dir ? -1 : 1;
        return *this;
    }

    Iterator &operator --()
    {
        ptr_ -= inv_dir ? -1 : 1;
        return *this;
    }

    T& operator *() const
    {
        return *ptr_;
    }
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
private:
    Alloc alloc_;
    T *arr_;
    size_t curr_size;
    size_t max_curr_size;
public:
    using iterator = Iterator<T>;
    Vector(size_t sz=0):curr_size(0), arr_(alloc_.allocate(sz)), max_curr_size(sz)
    {
    }

    T& operator[](size_t index)
    {
        return arr_[index];
    }

    ~Vector()
    {
        clear();
        alloc_.deallocate(arr_, max_curr_size);
    }

    void push_back(T elem)
    {
        if(curr_size == max_curr_size)
        {
            reserve(max_curr_size * 2 + 1);
        }
        arr_[curr_size] = elem;
        curr_size++;
    }

    void pop_back()
    {
        curr_size--;
        alloc_.destroy(arr_ + curr_size);
    }

    void reserve(size_t new_size)
    {
        if(new_size > max_curr_size)
        {
            T *arr = alloc_.allocate(new_size+1);
            for(size_t i = 0; i < curr_size; i++)
            {
                alloc_.construct(arr + i, std::forward<T>(*(arr_ + i)));
                alloc_.destroy(arr_ + i);
            }
            alloc_.deallocate(arr_, max_curr_size);
            arr_ = arr;
            max_curr_size = new_size;
        }
    }

    void resize(size_t new_size)
    {
        if(new_size > curr_size)
        {
            reserve(new_size);
            for(auto i = curr_size; i < new_size; i++)
            {
                alloc_.construct(arr_ + i);
            }
        }
        else
            for(auto i = new_size; i < curr_size; i++)
            {
                alloc_.destroy(arr_ + i);
            }
        curr_size = new_size;
    }

    void clear()
    {
        for(size_t i = 0; i < curr_size; i++)
            alloc_.destroy(arr_ + i);
        curr_size = 0;
    }

    size_t size()
    {
        return curr_size;
    }

    size_t capacity() const
    {
        return max_curr_size;
    }

    bool empty()
    {
        return curr_size == 0;
    }

    iterator begin()
    {
        return Iterator<T>(arr_);
    }

    iterator end()
    {
        return Iterator<T>(arr_ + curr_size);
    }

    iterator rbegin()
    {
        return Iterator<T>(arr_ + curr_size - 1, true);
    }

    iterator rend()
    {
        return  Iterator<T>(arr_ - 1, true);
    }
};
