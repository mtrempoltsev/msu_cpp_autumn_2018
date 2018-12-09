#pragma once

#include <cstddef>
#include <limits>
#include <iterator>

template <typename T>
class Allocator {
public:
	using value_type = T;
	using pointer = T *;
	using size_type = size_t;

	pointer allocate(size_type const count) {
		return static_cast<pointer>(::operator new(sizeof(value_type ) * count));
	}

	template <typename ... Args>
	void construct(pointer ptr, Args && ... args) {
		new (ptr) value_type(std::forward<Args>(args) ...);
	}

	void deallocate(pointer ptr) {
		::operator delete(ptr);
	}

	void destroy(pointer ptr) {
		ptr->~value_type();
	}

};

template <typename T>
class Iterator 
	: public std::iterator<std::random_access_iterator_tag, T> {
public:
	using reference = T &;
	using pointer = T *;
	using iterator = Iterator<T>;

private:
	pointer ptr_;

public:
	Iterator(pointer ptr) : ptr_(ptr) {}

	bool operator==(Iterator<T> const & other) const {
		return ptr_ == other.ptr_;
	}

	bool operator!=(Iterator<T> const & other) const {
		return !(*this == other);
	}

	reference operator*() const {
		return *ptr_;
	}

	iterator & operator++() {
		ptr_++;
		return *this;
	}

	iterator & operator--() {
		ptr_--;
		return *this;
	}
};

template <typename T, typename Alloc = Allocator<T>>
class Vector {
public:
	using size_type = size_t;
	using pointer = T *;
	using value_type = T;
	using reference = T &;
	using const_reference = T const &;
	using iterator = Iterator<T>;
	using reverse_iterator = std::reverse_iterator<Iterator<T>>;

private:
	Alloc alloc_;
	pointer data_;
	size_type size_;
	size_type capacity_;

public:
	Vector() : size_(0), capacity_(8) {
		data_ = alloc_.allocate(capacity_);
	}

	~Vector() {
		clear();
		alloc_.deallocate(data_);
	}

	void clear() {
		resize(0);
	}

	reference operator[](size_t idx) {
		if (idx < size_)
			return data_[idx];
		throw std::out_of_range("out of range!");
	}

	const_reference operator[](size_t idx) const {
		if (idx < size_)
			return data_[idx];
		throw std::out_of_range("out of range!");
	}

	void reserve(size_type const count) {
		if (capacity_ < count) {
			pointer new_data = alloc_.allocate(count);

			for (size_type i = 0; i < size_; i++) {
				alloc_.construct(new_data + i, std::move(*(data_ + i)));
				alloc_.destroy(data_ + i);
			}

			alloc_.deallocate(data_);
			capacity_ = count;
			data_ = new_data;
		}
	}

	void resize(size_type const count) {
		if (size_ > count) {
			for (size_type i = count; i < size_; i++)
				alloc_.destroy(data_ + i);
		} else {
			if (capacity_ < count)
				reserve(count);

			for (size_type i = size_; i < count; i++)
				alloc_.construct(data_ + i);
		}

		size_ = count;
	}

	void push_back(const_reference value) {
		if (size_ >= capacity_) {
			reserve(capacity_ * 2);
		}

		alloc_.construct(data_ + size_++, value);
	}

	void push_back(T && value) {
		if (size_ >= capacity_) {
			reserve(capacity_ * 2);
		}

		alloc_.construct(data_ + size_++, value);
	}

	void pop_back() {
		if (size_ != 0) {
			alloc_.destroy(data_ + size_);
			size_--;
		} else {
			throw std::out_of_range("vector is empty!");
		}
	}

	constexpr size_type const & size() const noexcept {
		return size_;
	}

	constexpr size_type const & capacity() const noexcept {
		return capacity_;
	}

	constexpr bool empty() const noexcept {
		return size_ == 0;
	}

	constexpr iterator begin() const noexcept {
		return iterator(data_);
	}

	constexpr iterator end() const noexcept {
		return iterator(data_ + size_);
	}

	constexpr reverse_iterator rbegin() const noexcept {
		return reverse_iterator(end());
	}

	constexpr reverse_iterator rend() const noexcept {
		return reverse_iterator(begin());
	}

};