#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template <class T>
class Allocator
{
public:
	T* allocate(const int n_elem)
	{
		//T* ptr = new T[n_elem];
		T* ptr = (T*)malloc(sizeof(T)*n_elem);
		return ptr;
	};
	template <class... Args>
	void create(T* ptr, Args&&... arg)
	{
		ptr = new(ptr) T(forward<Args>(arg)...);
	};
	void remove(T* ptr)
	{
		ptr->~T();
	};
	void deallocate(T* ptr)
	{
		//delete[] ptr;
		free(ptr);
	};
};

template <class T>
class Iterator : public iterator<random_access_iterator_tag, T>
{
private:
	T *ptr_;
public:
	using iterator = Iterator<T>;
	Iterator(T* ptr)
	{
		ptr_ = ptr;
	};
	Iterator(const iterator& iter)
	{
		ptr_ = iter.ptr_;
	};
	bool operator==(const iterator& iter)
	{
		return ptr_ == iter.ptr_;
	};
	bool operator!=(const iterator& iter)
	{
		return !(*this == iter);
	};
	bool operator<(const iterator& iter)
	{
		return ptr_ < iter.ptr_;
	};
	bool operator>(const iterator& iter)
	{
		return ptr_ > iter.ptr_;
	};
	T& operator*()
	{
		return *ptr_;
	};
	iterator& operator++()
	{
		++ptr_;
		return *this;
	};
	iterator& operator--()
	{
		--ptr_;
		return *this;
	};
	iterator& operator+=(const int i)
	{
		ptr_ += i;
		return *this;
	};
	iterator& operator-=(const int i)
	{
		ptr_ -= i;
		return *this;
	};
	iterator operator+(const int i)
	{
		iterator iter(ptr_ + i);
		return iter;
	};
	iterator operator-(const int i)
	{
		iterator iter(ptr_ - i);
		return iter;
	};
	iterator operator+(const iterator& iter)
	{
		iterator new_iter(ptr_ + iter.ptr_);
		return new_iter;
	};
	iterator operator-(const iterator& iter)
	{
		iterator new_iter(ptr_ - iter.ptr_);
		return new_iter;
	};
	friend iterator operator+(const int i, const iterator& iter)
	{
		iterator new_iter(iter.ptr_ + i);
		return new_iter;
	};
	friend iterator operator-(const int i, const iterator& iter)
	{
		iterator new_iter(i - iter.ptr_);
		return new_iter;
	};
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
	using iterator = Iterator<T>;
	using reverse_iterator = std::reverse_iterator<iterator>;

	Vector(const int n_elem = 0)
	{
		data_ = alloc_.allocate(n_elem);
		capacity_ = n_elem;
		resize(n_elem);
	};
	~Vector()
	{
		clear();
		alloc_.deallocate(data_);
	};
	T& operator[](const int i)
	{
		if (i <= size_)
		{
			if (i == size_)
				++size_;
			return data_[i];
		}
		else
			throw out_of_range("");
	};
	const T operator[](const int i) const
	{
		if (i <= size_)
			return data_[i];
		else
			throw out_of_range("");
	};
	void push_back(const T& value)
	{
		if (capacity_ == size_)
		{
			if (capacity_ == 0)
				reserve(1);
			else
				reserve(capacity_ * 2);
		}
		alloc_.create(data_ + size_, value);
		++size_;
	};
	void pop_back()
	{
		if (size_ > 0)
		{
			alloc_.remove(data_ + size_ - 1);
			--size_;
		}
		else
			throw out_of_range("");
	};
	bool empty() const
	{
		return (size_ == 0);
	};
	const int size() const
	{
		return size_;
	};
	const int capacity() const
	{
		return capacity_;
	};
	void resize(const int n_elem)
	{
		if (capacity_ < n_elem)
		{
			reserve(n_elem);
			for (int i = size_; i < (n_elem - size_); ++i)
				alloc_.create(data_ + i);
		}
		else
		{
			if (size_ > n_elem)
				for (int i = n_elem; i < size_; ++i)
					alloc_.remove(data_ + i);
			else
				for (int i = size_; i < n_elem; ++i)
					alloc_.create(data_ + i);
		}
		size_ = n_elem;
	};
	void reserve(const int n_elem)
	{
		if (n_elem > capacity_)
		{
			T* new_ptr = alloc_.allocate(n_elem);
			for (int i = 0; i < size_; ++i)
				alloc_.create(new_ptr + i, data_[i]);
			for (int i = 0; i < size_; ++i)
				alloc_.remove(data_ + i);
			alloc_.deallocate(data_);
			data_ = new_ptr;
		}
		else
		{
			for (int i = n_elem; i < capacity_; ++i)
				alloc_.remove(data_ + i);
			if (size_ > n_elem)
				size_ = n_elem;
		};
		capacity_ = n_elem;
	};
	void clear()
	{
		resize(0);
	};
	iterator begin()
	{
		return iterator(data_);
	};
	iterator end()
	{
		return iterator(data_ + size_);
	};
	reverse_iterator rbegin()
	{
		return reverse_iterator(data_ + size_);
	};
	reverse_iterator rend()
	{
		return reverse_iterator(data_);
	};
private:
	T *data_;
	size_t size_;
	size_t capacity_;
	Alloc alloc_;
};