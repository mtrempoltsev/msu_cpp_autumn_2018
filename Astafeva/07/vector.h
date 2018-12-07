#pragma once

#include <iterator>

template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type n) {
        return static_cast<pointer>(::operator new(sizeof(value_type) * n));
    }
    void deallocate(pointer p) {
        ::operator delete(p);
    }
    template<class... ArgsT>
    void construct(pointer p, ArgsT&&... args){
        ::new(p) T(std::forward<ArgsT>(args)...);
    }
    void destroy(pointer p) {
        p->~value_type();
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;

private:
    pointer p;

public:
    explicit Iterator(pointer p) : p(p) {}
    reference operator*() const {
        return *p;
    }
    void operator=(Iterator it){
        p = it.p;
    }
    void operator=(pointer pt){
        p = pt;
    }
    bool operator==(const Iterator<T>& it) const {
        return p == it.p;
    }
    bool operator!=(const Iterator<T>& it) const {
        return !(*this == it);
    }
    Iterator& operator++() {
        ++p;
        return *this;
    }
    Iterator& operator--() {
        --p;
        return *this;
    }
    Iterator& operator+=(size_type c) {
        for (size_type i = 0; i < c; ++i) {
            ++p;
        }
        return *this;
    }
    Iterator& operator-=(size_type c) {
        for (size_type i = 0; i < c; ++i) {
            --p;
        }
        return *this;
    }
    Iterator operator+(size_type c) const {
        return Iterator(p + c);
    }
    Iterator operator-(size_type c) const {
        return Iterator(p - c);
    }
    bool operator>(const Iterator<T>& it) const {
        return distance(it - p) > 0;
    }
    bool operator<(const Iterator<T>& it) const {
		return distance(it - p) < 0;
	}
	bool operator>=(const Iterator<T>& it) const {
		return distance(it - p) >= 0;
	}
	bool operator<=(const Iterator<T>& it) const {
		return distance(it - p) <= 0;
	}
    reference operator[](size_type i) {
        return p[i];
    }
};

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<Iterator<T>>;
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
private:
    Alloc all;
    pointer data;
    size_type sz;
    size_type all_sz;
public:
    explicit Vector(size_type count) : sz(count), all_sz(count << 1) {
        data = all.allocate(all_sz);
        for (size_type i = 0; i < sz; ++i) {
            all.construct(data + i);
        }
    }
    explicit Vector() : sz(0), all_sz(4) {
        data = all.allocate(all_sz);
    }
    Vector(const Vector& V) : sz(V.sz), all_sz(V.all_sz) {
        data = all.allocate(all_sz);
        for(size_type i = 0; i < sz; ++i)
            all.construct(data + i, V.data[i]);
    }
    Vector(Vector&& V) : data(V.data), sz(V.sz), all_sz(V.all_sz) {
        V.sz = 0;
        V.all_sz = 0;
        V.data = nullptr;
    }
    Vector& operator=(const Vector& V) {
        if (this == &V)
            return *this;
        pointer t = all.allocate(V.all_sz);
        for (size_type i = 0; i < V.sz; ++i)
                all.construct(t[i], V.data[i]);
        for (size_type i = 0; i < sz; ++i)
                all.destroy(data[i]);
        all.deallocate(data);
        sz = V.sz;
        all_sz = V.all_sz;
        data = t;
        return *this;
    }
    void clear() noexcept {
        for (size_type i = 0; i < sz; ++i) {
            all.destroy(data + i);
        }
        sz = 0;
        all_sz = 0;
    }
    ~Vector() {
        clear();
        all.deallocate(data);
    }
    reference operator[] (size_type id) {
        return data[id];
    }
    const reference operator[] (size_type id) const {
        return data[id];
    }
    bool empty() const {
        return sz == 0;
    }
    size_type size() const {
        return sz;
    }
    size_type capacity() const {
        return all_sz;
    }
    void push_back(value_type&& value) {
        if (sz == all_sz)
            reserve(all_sz << 1);
        all.construct(data + sz, std::move(value));
        ++sz;
    }
    void push_back(const value_type& value) {
        if (sz == all_sz)
            reserve(all_sz << 1);
        all.construct(data + sz, value);
        ++sz;
    }
    void pop_back() {
        if (sz > 0) {
            --sz;
            all.destroy(data + sz);
        }
    }
    const reference front() const {
        if(sz != 0)
            return data[0];
        throw std::out_of_range("");
    }
    const reference back() const {
        if(sz != 0)
            return data[sz - 1];
        throw std::out_of_range("");
    }
    const reference front() {
        if(sz != 0)
            return data[0];
        throw std::out_of_range("");
    }
    const reference back() {
        if(sz != 0)
            return data[sz - 1];
        throw std::out_of_range("");
    }
    void reserve(size_type count) {
        if (all_sz < count) {
            pointer ndata = all.allocate(count);
            for (size_type i = 0; i < sz; ++i) {
                all.construct(ndata + i, *(data + i));
                all.destroy(data + i);
            }
            all.deallocate(data);
            data = ndata;
            all_sz = count;
        }
    }
    void resize(size_type newSize) {
        if (sz > newSize) {
            for (size_type i = newSize; i < sz; ++i) {
                all.destroy(data + i);
            }
        } else {
            if (all_sz < newSize)
                this->reserve(newSize << 1);
            for (size_type i = sz; i < newSize; i++)
                all.construct(data + i);
        }
        sz = newSize;
    }
    iterator begin() const noexcept {
        return iterator(data);
    }
    iterator end() const noexcept {
        return iterator(data + sz);
    }
    reverse_iterator rbegin() const noexcept {
        return reverse_iterator(end());
    }
    reverse_iterator rend() const noexcept {
        return reverse_iterator(begin());
    }
};
