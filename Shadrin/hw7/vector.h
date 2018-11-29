#include <iterator>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T *;
    using size_type = std::size_t;

    pointer allocate(size_type num) {
        return static_cast<pointer>(::operator new[](num * sizeof(size_type)));
    }

    void deallocate(pointer ptr, size_type num) {
        ::operator delete[](ptr, num * sizeof(value_type));
    }

    template<class... Args>
    void construct(pointer p, Args&&... args) {
        ::new(p) T(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        p->~T();
    }
};

template <class T>
class Iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{

    T *current_;

public:
    using reference = T&;

    Iterator(T *data)
        : current_(data)
    { }

    bool operator ==(const Iterator& other) const {
        return current_ == other.current_;
    }

    bool operator !=(const Iterator& other) const {
        return !(*this == other);
    }

    Iterator& operator ++() {
        ++current_;
        return *this;
    }

    Iterator& operator --() {
        --current_;
        return *this;
    }

    reference operator *() const {
        return *current_;
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<T>;

    explicit Vector(size_type count = 0)
        : alloc{}
        , cur_alloc_size{START_ALLOC_SIZE}
        , data{alloc.allocate(cur_alloc_size)}
        , cur_used_size{count} 
    {
        for (auto ptr = data; ptr != data + cur_used_size; ++ptr) {
            alloc.construct(ptr);
        }
    };

    reference operator[](size_type pos) {
        return data[pos];
    }
    const_reference operator[](size_type pos) const {
        return data[pos];
    }

    void push_back(value_type&& value) {
        if (cur_used_size >= cur_alloc_size) {
            reserve(cur_used_size * INCREASE_COEF);
        }
        alloc.construct(data + cur_used_size, std::forward<value_type>(value));
        ++cur_used_size;
    }
    void push_back(const value_type& value) {
        if (cur_used_size >= cur_alloc_size) {
            reserve(cur_used_size * INCREASE_COEF);
        }
        alloc.construct(data + cur_used_size, std::forward<value_type>(value));
        ++cur_used_size;
    }
    void pop_back() {
        if (cur_used_size > 0) {
            --cur_used_size;
            alloc.destroy(data + cur_used_size);
        }
    }

    size_type size() const {
        return cur_used_size;
    }
    size_type capacity() const {
        return cur_alloc_size;
    }
    bool empty() const {
        return (cur_used_size == 0);
    }
    void clear() {
        for (auto ptr = data; ptr != data + cur_used_size; ++ptr) {
            alloc.destroy(ptr);
        }
        cur_used_size = 0;
    }


    iterator begin() {
        return iterator{data};
    }
    std::reverse_iterator<iterator> rbegin() {
        return std::reverse_iterator<iterator>{iterator{data + cur_used_size}};
    }
    iterator end() {
        return iterator{data + cur_used_size};
    }
    std::reverse_iterator<iterator> rend() {
        return std::reverse_iterator<iterator>{iterator{data}};
    }

    void resize(size_type count) {
        if (count < cur_used_size) {
            for (auto ptr = data + count; ptr != data + cur_used_size; ++ptr) {
                alloc.destroy(ptr);
            }
            cur_used_size = count;
            return;
        }

        if (count < cur_alloc_size) {
            for (auto ptr = data + cur_used_size; ptr != data + count; ++ptr) {
                alloc.construct(ptr);
            }
            cur_used_size = count;
            return;
        }

        this->reserve(count);
        for (auto ptr = data + cur_used_size; ptr != data + count; ++ptr) {
            alloc.construct(ptr);
        }
        cur_used_size = count;
    }

    void reserve(size_type count) {
        if (count <= cur_alloc_size) {
            return;
        }
        pointer newdata = alloc.allocate(count);
        for (size_type ind = 0; ind < cur_used_size; ++ind) {
            alloc.construct(newdata + ind, std::forward<value_type>(*(data + ind)));
            alloc.destroy(data + ind);
        }
        alloc.deallocate(data, cur_alloc_size);
        data = newdata;
        cur_alloc_size = count;
    }

    ~Vector() {
        for (auto ptr = data; ptr != data + cur_used_size; ++ptr) {
            alloc.destroy(ptr);
        }
        alloc.deallocate(data, cur_alloc_size);
    }

private:
    static constexpr size_type START_ALLOC_SIZE = 1;
    static constexpr size_type INCREASE_COEF = 2;

    Alloc alloc;
    size_type cur_alloc_size;

    pointer data;
    size_type cur_used_size;
};
