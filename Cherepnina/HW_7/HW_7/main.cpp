#include <iostream>
#include <cstring>

template<class T>
class Allocator {
public:
    void reallocate(T **pointer, size_t size) {
        T* new_data = new T[size];
        memcpy(new_data, *pointer, sizeof(T) * (size - 1));
        delete[] *pointer;
        *pointer =  new_data;
    }

    T *allocate(size_t size) {
        return new T[size];
    }
};


template<class T>
class Iterator {
    T *current_;
    T *end_;
    int direction;

    void findNext() {
        if (current_ != end_)
            current_ += direction;
    }

public:
    Iterator(T *first, T *last, int direction = 1) : current_(first),
                                                     end_(last),
                                                     direction(direction) {}

    bool operator==(const Iterator &other) const {
        return *current_ == *other.current_;
    }

    bool operator!=(const Iterator &other) const {
        return !(*this == other);
    }

    void operator++() {
        if (current_ != end_) {
            findNext();
        }
    }

    T operator*() const {
        return *current_;
    }

};

template<class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;

    Vector() : size_(0), capacity_(0), alloc_{} {
        data = alloc_.allocate(size_);
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    void reserve(size_t new_size) {
        if (new_size <= capacity_)
            return;
        alloc_.reallocate(&data, new_size);
        capacity_ = new_size;
    }

    void resize(size_t count) {
        if (count <= size_) {
            size_ = count;
            return;
        }
        if (count > capacity_)
            if (count > capacity_ * 2)
                reserve(count);
            else
                reserve(capacity_ * 2);

        for (size_t i = size_; i < capacity_; ++i)
            new(data+i)T;
        size_ = count;
    }

    size_t capacity() {
        return capacity_;
    }

    void push_back(T &&arg) {
        if (capacity_ > size_) {
            new(data + size_)T(arg);
            ++size_;
            return;
        }
        alloc_.reallocate(&data, ++size_);
        capacity_ = size_;
        new(data + size_ - 1)T(arg);
    }

    void pop_back() {
        --size_;
    }

    T &operator[](int num) {
        return data[num];
    }

    iterator rbegin() const {
        return iterator(&data[size_ - 1], &data[-1], -1);
    }

    iterator rend() const {
        return iterator(&data[-1], &data[-1]);
    }

    iterator begin() const {
        return iterator(&data[0], &data[size_], 1);
    }

    iterator end() {
        return iterator(&data[size_], &data[size_]);
    }

    ~Vector() {
        delete[] data;
    }

private:
    T *data;
    size_t capacity_;
    size_t size_;
    Alloc alloc_;
};


#include <chrono>
#include <iostream>

#include <deque>
#include <list>
#include <vector>

template<class T>
int benchmark(T &c) {
    const int N = 1000000;

    int res = 0;

    for (int j = 0; j < 20; ++j) {
        for (int i = 0; i < N / 2; ++i) {
            c.push_back(i + j);
        }

        for (int i : c) {
            res += i;
        }

        c.resize(N / 4);

        for (int i : c) {
            res += i;
        }

        c.resize(N);

        for (int i : c) {
            res += i;
        }

        for (int i = 0; i < N / 2; ++i) {
            c.pop_back();
        }

        for (int i : c) {
            res += i;
        }

        for (int i = 0; i < N / 2; ++i) {
            c.push_back(i + j);
        }

        for (int i : c) {
            res += i;
        }

        c.clear();
    }

    return res;
}

template<class X, class Y>
void doCheckEqual(const X &actual, const Y &expected, size_t line) {
    if (actual != expected) {
        std::cout << "at line " << line << ": " << actual << " != " << expected << '\n';
    }
}

#define checkEqual(x, y) do { doCheckEqual((x), (y), __LINE__); } while(0)
#define checkTrue(cond) do { if (!(cond)) std::cout << "at line " << __LINE__ << ": " << #cond << '\n'; } while(0)

static int Counter = 0;

struct Counterable {
    Counterable() {
        ++Counter;
    }

    Counterable(const Counterable &) {
        ++Counter;
    }

    Counterable &operator=(const Counterable &) {
        ++Counter;
        return *this;
    }

    ~Counterable() {
        --Counter;
    }
};

class Timer {
public:
    Timer()
            : start_(std::chrono::high_resolution_clock::now()) {
    }

    ~Timer() {
        const auto finish = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(finish - start_).count() << " us"
                  << std::endl;
    }

private:
    const std::chrono::high_resolution_clock::time_point start_;
};

int main() {

    {
        Vector<Counterable> v;
        checkEqual(Counter, 100);
        v.resize(1);


        checkEqual(Counter, 100);

        for (int i = 0; i < 100; ++i) {
            v.push_back(Counterable());
        }

        checkEqual(Counter, 200);

//        v.resize(150);
//
//        checkEqual(Counter, 150);
//
//        for (int i = 0; i < 100; ++i) {
//            v.pop_back();
//        }
//
//        checkEqual(Counter, 50);
//
//        v.resize(25);
//
//        checkEqual(Counter, 25);
//
//        v.clear();
//
//        checkEqual(Counter, 0);
//
//        v.resize(25);
//
//        checkEqual(Counter, 25);
//    }
//
//    checkEqual(Counter, 0);
//
//    int res = 0;
//
//    {
//        std::vector<int> v;
//        res += benchmark(v);
//    }
//
//    {
//        std::cout << "Vector<int>: ";
//        Timer t;
//        Vector<int> v;
//        res += benchmark(v);
//    }
//
//    {
//        std::cout << "std::vector<int>: ";
//        Timer t;
//        std::vector<int> v;
//        res += benchmark(v);
//    }
//
//    {
//        std::cout << "std::deque<int>: ";
//        Timer t;
//        std::deque<int> v;
//        res += benchmark(v);
//    }
//
//    {
//        std::cout << "std::list<int>: ";
//        Timer t;
//        std::list<int> v;
//        res += benchmark(v);
    }

//    return res;
}
