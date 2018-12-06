#include <iterator>

template<class T> 
class Allocator
{
public:
    T* allocate(size_t n) {
        return static_cast<T*> (::operator new(n * sizeof (T)));
    }

    void deallocate(T* ptr, size_t sz) {
        ::operator delete[](ptr, sizeof(T) * sz);
    }

    template<class ... Args>
    void construct(T* ptr, Args&&... args) {
        ::new(ptr) T(std::forward<Args>(args)...);
    }

    void destroy(T* ptr) {
        ptr->~T();
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
    T* ptr;
public:
    explicit Iterator(T* ptr_) : ptr(ptr_) {}

    T& operator*() const {
        return *ptr;
    }

    bool operator==(const Iterator& a) const {
        return ptr == a.ptr;
    }

    bool operator!=(const Iterator& a) const {
        return !(*this == a);
    }

    Iterator& operator++() {
        ptr++;
        return *this;
    }

    Iterator& operator--() {
        ptr--;
        return *this;
    }

    Iterator& operator+=(int n) {
        ptr += n;
        return *this;
    }

    Iterator& operator-=(int n) {
        return *this += -n;
    }

    Iterator& operator[](int val) {
        return *(ptr + val);
    }

    bool operator < (const Iterator& a) const {
        return (a - *this > 0);
    }

    bool operator > (const Iterator& a) const {
        return (a - *this < 0);
    }

    bool operator >= (const Iterator& a) const {
        return !(*this < a);
    }

    bool operator <= (const Iterator& a) const {
        return !(*this > a);
    }
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
    Alloc alloc;
    T* data;
    size_t size_;
    size_t max_size;
public:
    using iterator = Iterator<T>;
    Vector(size_t size = 0) : alloc{} {
        size_ = size;
        max_size = ((size!=0)?(2 * size):(1));
        data = alloc.allocate(max_size);
        for (auto ptr = data; ptr != data + size_; ptr++)
            alloc.construct(ptr);
    }
    ~Vector() {
        for (auto i = data; i != data + size_; i++)
            alloc.destroy(i);
        alloc.deallocate(data, max_size);
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return max_size;
    }

    bool empty() const {
        return (size_ == 0);
    }

    T& operator[](size_t pos) {
        if (pos < size_)
            return data[pos];
        throw std::out_of_range("???");
    }

    void resize(size_t new_size) {
        if (new_size < size_) {
            for (auto i = data + new_size; i != data + size_; i++)
                alloc.destroy(i);
        } else if (new_size > size_){
            this->reserve(new_size);
            for (auto i = data + size_; i != data + new_size; i++)
                alloc.construct(i);
        }
        size_ = new_size;
    }

    void reserve(size_t new_size) {
        if (new_size <= max_size)
            return;

        T* new_mas = alloc.allocate(new_size);
        for (auto i = 0; i < size_; i++) {
            alloc.construct(new_mas + i, std::forward<T>(*(data + i)));
            alloc.destroy(data + i);
        }
        alloc.deallocate(data, max_size);
        data = new_mas;
        max_size = new_size;
    }

    void push_back(T&& value) {
        if (size_ >= max_size)
            reserve(size_ * 2);
        alloc.construct(data + size_, std::move(value));
        size_++;
    }

    void push_back(const T& value)
    {
        if (size_ >= max_size)
            reserve(size_ * 2);
        alloc.construct(data + size_, value);
        size_++;
    }

    void pop_back()
    {
        if (size_ > 0) {
            alloc.destroy(data + size_ - 1);
            size_--;
        }
    }

    void clear()
    {
        for (auto i = data; i != data + size_; i++)
            alloc.destroy(i);
        size_ = 0;
    }

    iterator begin() const {
        return iterator(data);
    }

    iterator end() const {
        return iterator(data + size_);
    }

    std::reverse_iterator<iterator> rbegin() const {
        return std::reverse_iterator<iterator>(end());
    }

    std::reverse_iterator<iterator> rend() const {
        return std::reverse_iterator<iterator>{begin()};
    }
};

