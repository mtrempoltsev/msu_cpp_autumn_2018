#include <iterator>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type count) {
        return static_cast<pointer>(::operator new[](count * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type count) {
        ::operator delete[](ptr, count * sizeof(value_type));
    }

    template<class... Args>
    void construct(pointer ptr, Args&&... args) {
        new(ptr) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }

    size_type max_size() const{
        return std::numeric_limits<size_type>::max();
    }

};


template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using reference = T&;
    using pointer = T*;
    explicit Iterator(pointer ptr) : ptr(ptr) {}

    bool operator==(const Iterator& it) const
    {
        return ptr == it.ptr;
    }

    bool operator!=(const Iterator& it) const
    {
        return !(*this == it);
    }

    reference operator*() const
    {
        return *ptr;
    }

    Iterator& operator++()
    {
        ++ptr;
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }
    
    Iterator& operator--()
    {
        --ptr;
        return *this;
    }

    Iterator operator--(int)
    {
        Iterator temp = *this;
        --(*this);
        return temp;
    }

    Iterator& operator+=(int n)
    {
        ptr += n;
        return *this;
    }
    
    Iterator operator+(int n)
    {
        Iterator temp = *this;
        return temp += n;
    }
    
    Iterator& operator-=(int n)
    {
        return *this += -n;
    }

    Iterator operator-(int n)
    {
        Iterator temp = *this;
        return temp -= n;
    }
    
    std::ptrdiff_t operator-(const Iterator& it)
    {
        return ptr - it.ptr;
    }

    Iterator& operator[](int val)
    {
        return *(ptr + val);
    }

    bool operator < (const Iterator& it)
    {
        return (it - *this > 0);
    }
    
    bool operator > (const Iterator& it)
    {
        return it < *this;
    }
    
    bool operator >= (const Iterator& it)
    {
        return !(*this < it);
    }
    
    bool operator <= (const Iterator& it)
    {
        return !(*this > it);
    }
private:
    pointer ptr;
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using const_reference = const T&;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;

    Vector() 
        : alloc{}
        , data(nullptr)
        , used_size(0)
        , alloc_size(0) {}

    Vector(size_type size) 
        : alloc{}
        , used_size(size)
        , alloc_size(2 * size)
        , data(alloc.allocate(alloc_size))
    {
        for (auto ptr = data; ptr != data + used_size; ptr++) {
            alloc.construct(ptr);
        }
    }

    ~Vector() {
        for (auto ptr = data; ptr != data + used_size; ptr++) 
        {
            alloc.destroy(ptr);
        }
        alloc.deallocate(data, alloc_size);
    }
    
    reference operator[](size_type pos) const 
    {
        return data[pos];
    }

    size_type size() const 
    {
        return used_size;
    }

    size_type capacity() const 
    {
        return alloc_size;
    }

    bool empty() const 
    {
        return (used_size == 0);
    }

    void reserve(size_type new_size) 
    {
        if (new_size <= alloc_size) {
            return;
        }
        pointer buf = alloc.allocate(new_size);
        for (size_type pos = 0; pos < used_size; pos++) {
            alloc.construct(buf + pos, std::forward<value_type>(*(data + pos)));
            alloc.destroy(data + pos);
        }
        alloc.deallocate(data, alloc_size);
        data = buf;
        alloc_size = new_size;
    }

    void resize(size_type new_size) 
    {
        if (new_size < used_size) {
            for (auto ptr = data + new_size; ptr != data + used_size; ptr++) {
                alloc.destroy(ptr);
            }
        } else if (new_size > used_size){
            this->reserve(new_size);
            for (auto ptr = data + used_size; ptr != data + new_size; ptr++) {
                alloc.construct(ptr);
            }
        }
        used_size = new_size;
    }

    void push_back(value_type&& value) 
    {
        if (used_size >= alloc_size) {
            size_type new_size = (alloc_size == 0 ? 1 : used_size * 2);
            reserve(new_size);
        }
        alloc.construct(data + used_size, std::forward<value_type>(std::move(value)));
        used_size++;
    }

    void push_back(const_reference value) 
    {
        if (used_size >= alloc_size) {
            size_type new_size = (alloc_size == 0 ? 1 : used_size * 2);
            reserve(new_size);
        }
        alloc.construct(data + used_size, value);
        used_size++;
    }

    void pop_back() 
    {
        if (used_size > 0) {
            alloc.destroy(data + used_size - 1);
            used_size--;
        }
    }
    
    void clear() 
    {
        for (auto ptr = data; ptr != data + used_size; ptr++) {
            alloc.destroy(ptr);
        }
        used_size = 0;
    }

    iterator begin() const 
    {
        return iterator(data);
    }

    iterator end() const 
    {
        return iterator(data + used_size);
    }

    std::reverse_iterator<iterator> rbegin() const 
    {
        return std::reverse_iterator<iterator>(end());
    }
    
    std::reverse_iterator<iterator> rend() const 
    {
        return std::reverse_iterator<iterator>{begin()};
    }
private:
    Alloc alloc;
    pointer data;
    size_type used_size;
    size_type alloc_size;
};
