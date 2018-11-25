#include <iterator>
#include <stdexcept>

using namespace std;

template<class T>
class Allocator {
public:
	typedef T value_type;
	typedef value_type *pointer;
	typedef size_t size_type;
	pointer allocate(size_type size) {
		return static_cast<pointer>(::operator new[](size
						* sizeof(value_type)));
	}
	void deallocate(pointer ptr){::operator delete(ptr);}
	template<class... ArgsT>
	void construct(pointer ptr, ArgsT&&... Args){
		::new(ptr) value_type{forward<ArgsT>(Args)...};
	}
	void destroy(pointer ptr){ptr->~value_type();}
};

template<class T>
class Iterator: public iterator<forward_iterator_tag, T> {
public:
	typedef T value_type;
	typedef value_type *pointer;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef Iterator<value_type> iterator;
	typedef const iterator const_iterator;
private:
	pointer ptr;
	bool forward_direction;
public:
	explicit Iterator(pointer ptr, bool direct = true)
		: ptr(ptr)
		, forward_direction(direct) {};
	reference operator*() const {return *ptr;}
	bool operator==(const_iterator& other) const {
		return (ptr == other.ptr);
	}
	bool operator!=(const_iterator& other) const {
		return !((*this) == other);
	}
	Iterator& operator++() {
		if (forward_direction)
			ptr++;
		else
			ptr--;
		return *this;
	}
	Iterator& operator--() {
		if (forward_direction)
			ptr--;
		else
			ptr++;
		return *this;
	}
};

template<class T, class Allocat = Allocator<T>>
class Vector {
public:
	typedef T value_type;
	typedef value_type *pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type&& rvalue_reference;
	typedef size_t size_type;
	typedef Iterator<value_type> iterator;
private:
	static const size_type BUF_SIZE = 1024;
	Allocat A;
	pointer data;
	size_type size_;
	size_type alloc_size;
public:
	explicit Vector(size_type n = 0)
		: data(A.allocate(n))
		, size_(n)
		, alloc_size(n + BUF_SIZE - (n % BUF_SIZE)) {
			data = A.allocate(alloc_size);
			for (size_t i = 0; i < size_; i++)
				A.construct(data + i);
	}
	Vector(const Vector& other)
		: Vector(other.data, other.size_, other.alloc_size) {}
	Vector(Vector&& other)
		: data(other.data)
		, size_(other.size_)
		, alloc_size(other.alloc_size) {
		other.data = nullptr;
		other.size_ = 0;
		other.alloc_size = 0;
	}
	Vector& operator=(const Vector& other) {
		if (this == &other)
			return *this;
		pointer tmp = A.allocate(other.alloc_size);
		for (size_t i = 0; i < size_; i++)
			A.construct(tmp[i], other.data[i]);
		for (size_t i = 0; i < size_; i++)
			A.destroy(data[i]);
		A.deallocate(data);
		size_ = other.size_;
		alloc_size = other.alloc_size;
		data = tmp;
		return *this;
	}
	~Vector() {
		for (size_t i = 0; i < size_; i++)
			A.destroy(data + i);
		A.deallocate(data);
	}
	reference operator[](size_type index) {return data[index];}
	const_reference operator[](size_type index) const {return data[index];}
	reference at(size_type index) {
		if ((index < size_) && (index >= 0))
			return data[index];
		throw out_of_range("Error: index");
	}
	const_reference at(size_type index) const {
		if ((index < size_) && (index >= 0))
			return data[index];
		throw out_of_range("Error: index");
	}
	reference front() {
		if (size_)
			return data[0];
		throw out_of_range("Error: index");
	}
	const_reference front() const {
		if (size_)
			return data[0];
		throw out_of_range("Error: index");
	}
	reference back() {
		if (size_)
			return data[size_ - 1];
		throw out_of_range("Error: index");
	}
	const_reference back() const {
		if (size_)
			return data[size_ - 1];
		throw out_of_range("Error: index");
	}
	void push_back(rvalue_reference value) {
		if (alloc_size == size_)
			this->reserve(alloc_size + BUF_SIZE);
		A.construct(data + size_, value);
		size_++;
	}
	void pop_back() {
		if (!size_)
			throw out_of_range("Error: no elements");
		resize(size_ - 1);
	}
	size_type size() const {return size_;}
	bool empty() const {return !size_;}
	void clear() {
		for (size_type i = 0; i < size_; i++)
			A.destroy(data + i);
		size_ = 0;
	}
	iterator begin() {return iterator(data);}
	iterator rbegin() {return iterator(data + size_ - 1, false);}
	iterator end() {return iterator(data + size_);}
	iterator rend() {return iterator(data - 1, false);}
	void reserve(size_type n) {
		if (alloc_size < n) {
			pointer tmp = A.allocate(n);
			for (size_type i = 0; i < size_; i++) {
				A.construct(tmp + i, data[i]);
				A.destroy(data + i);
			}
			A.deallocate(data);
			data = tmp;
			alloc_size = n;
		}
	}
	size_type capacity() {return alloc_size;}
	void resize(size_type n) {
		if (size_ > n)
			for (size_type i = n; i < size_; i++)
				A.destroy(data + i);
		else {
			if (alloc_size < n)
				this->reserve(n + BUF_SIZE - (n % BUF_SIZE));
			for (size_type i = size_; i < n; i++)
				A.construct(data + i);
		}
		size_ = n;
	}
};
