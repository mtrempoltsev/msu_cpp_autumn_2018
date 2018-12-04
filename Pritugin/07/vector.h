/* Created by Michael Pritugin */

#include <iostream>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <vector>
/* 
 * What do i need in Vector:
 * 1) Vector(...) + test
 * 2) ~Vector() + test
 * 3) operator = +
 * 4) operator [] + test
 * 5) front() + test
 * 6) back() + test
 * 7) data() - don`t need
 * 8) empty() + test
 * 9) size() + test
 * 10) reserve() +
 * 11) push_back + test
 * 12) pop_back + test
 * 13) operator > + test
 * 14) operator < + test
 * 15) operator >= +
 * 16) operator <= +
 * 17) operator == + test
 * 18) operator != + test
 * 19) Vector(const Vector& other) +
 * 20) Vector(const Vector&& other) +
 * 21) std::cout << + test
*/

template <class T>
class Allocator
{
public:
	using value_type = T;
	using pointer = T *;
	using size_type = size_t;

	pointer allocate(size_type count)
	{
		return static_cast<pointer>(::operator new(count * sizeof(value_type)));
	}

	void deallocate(pointer ptr, size_type count)
	{
		::operator delete(ptr);
	}

	template <class... Args>
	void construct(T *ptr, Args&&... args)
	{
		::new (ptr) T(std::forward<Args>(args)...);
	}

	void destroy(pointer ptr)
	{
		ptr->~T();
	}

	size_type max_size() const noexcept
	{
		return std::numeric_limits<size_type>::max();
	}
};



template <class T>
class Iterator
	: public std::iterator<std::random_access_iterator_tag, T>
{
	template<T> friend  Iterator<T> operator+(int n, Iterator<T> iter);
	T* ptr_;
public:
	using reference = T&;
	typedef std::random_access_iterator_tag iterator_category;
	typedef std::ptrdiff_t difference_type;
	explicit Iterator(T* ptr)
		: ptr_(ptr)
	{
	}

	bool operator==(const Iterator<T>& other) const
	{
		return ptr_ == other.ptr_;
	}

	bool operator!=(const Iterator<T>& other) const
	{
		return !(*this == other);
	}

	reference operator*() const
	{
		return *ptr_;
	}

	Iterator<T>& operator++()
	{
		++ptr_;
		return *this;
	}
	
	Iterator<T>& operator--()
	{
		--ptr_;
		return *this;
	}
	
	Iterator<T>& operator [] (int n)
	{
		return *(ptr_ + n);
	}
	
	
	//---------
	
	Iterator<T>& operator +=(int n)
	{
		ptr_ += n;
		return *this;
	}
	
	Iterator<T> operator +(int n)
	{
		auto temp = *this;
		return temp += n;
	}
	
	Iterator<T> operator -=(int n)
	{
		return *this += -n;
	}
	
	difference_type operator - (const Iterator<T>& other)
	{
		return ptr_ - other.ptr_;
	}
	
	bool operator < (const Iterator<T>& other)
	{
		return (operator- (other) > 0);
	}
	
	bool operator > (const Iterator<T>& other)
	{
		return other < *this;
	}
	
	bool operator >= (const Iterator<T>& other)
	{
		return !(*this < other);
	}
	
	bool operator <= (const Iterator<T>& other)
	{
		return !(*this > other);
	}
};


template <class T>
Iterator<T> operator+(int n, Iterator<T> iter)
{
	return Iterator<T>(iter.ptr_ + n);
}


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
	
private:
	T* _data_;
	size_type _size_;
	size_type _maxsize_;
	Alloc _alloc_;
public:
	// Standart constructor
	Vector(size_type size) : _size_(size), _maxsize_(size + 4)
	{
		_data_ = _alloc_.allocate(_maxsize_);
		for(size_type i = 0; i < size; ++i)
			_alloc_.construct(_data_[i]);
	}
	
	// Empty constructor
	Vector() : _size_(0), _maxsize_(4)
	{
		_data_ =  _alloc_.allocate(_maxsize_);
	}
	
	// Copy constructors
	Vector(const Vector& other) : _size_(other._size), _maxsize_(other._maxsize_)
	{
		_data_ = _alloc_.allocate(_maxsize_);
		for(size_type i = 0; i < _size_; ++i)
			_alloc_.construct(_data_ + i, other._data_[i]);
	}
	
	// Move constructors
	Vector(Vector&& other) : _size_(other._size_),
	_maxsize_(other._maxsize_), _data_(other._data_) 
	{
		other._size_ = 0;
		other._maxsize_ = 0;
		other._data_ = nullptr;
	}
	
	// Standart destructor
	~Vector()
	{
		clear();
		_alloc_.deallocate(_data_, _maxsize_);
	}
	
	// operator =
	Vector& operator = (const Vector& other)
	{
		pointer bufer = _alloc_.allocate(other._maxsize_);
		
		for(size_type i = 0; i < other._size_; ++i)
			_alloc_.construct(bufer + i, other._data_[i]);

		for(size_type i = 0; i < _size_; ++i)
			_alloc_.destroy(_data_ + i);
			
		_size_ = other._size_;
		_alloc_.deallocate(_data_, _maxsize_);
		_maxsize_ = other._maxsize_;
		_data_ = bufer;
		return *this;
	}
	
	void clear()
	{
		for(size_type i = 0; i < _size_; ++i)
			_alloc_.destroy(_data_ + i);
		_size_ = 0;
	}
	
	size_type size() const { return _size_; }
	
	// operator []
	reference operator [](size_type pos)
	{
		if (pos < _size_)
			return _data_[pos];
		throw std::out_of_range("Wrong index");
	}
	
	const_reference operator [](size_type pos) const
	{
		if (pos < _size_)
			return _data_[pos];
		throw std::out_of_range("Wrong index");
	}
	
	const_reference front() const
	{
		if(_size_ != 0)
			return _data_[0];
		throw std::out_of_range("Vector is empty");
	}
	
	const_reference back() const
	{
		if(_size_ != 0)
			return _data_[_size_ - 1];
		throw std::out_of_range("Vector is empty");
	}
	
	bool empty() const
	{
		return _size_ == 0;
	}

	void push_back(value_type&& elem)
	{
		if(_size_ < _maxsize_)
		{
			_alloc_.construct(_data_ + _size_++, std::forward<value_type>(std::move(elem)));
			return;
		}

		pointer bufer = _alloc_.allocate(_maxsize_ * 2);
		for(size_type i = 0; i < _size_; ++i)
		{
			_alloc_.construct(bufer + i, _data_[i]);
			_alloc_.destroy(_data_ + i);
		}
		
		_alloc_.construct(_data_ + _size_++, std::forward<value_type>(std::move(elem)));
		_alloc_.deallocate(_data_, _maxsize_);
		_maxsize_ *= 2;
		_data_ = bufer;
	}

	void push_back(const_reference elem)
	{
		if(_size_ < _maxsize_)
		{
			_alloc_.construct(_data_ + _size_++, elem);
			return;
		}

		pointer bufer = _alloc_.allocate(_maxsize_ * 2);
		for(size_type i = 0; i < _size_; ++i)
		{
			_alloc_.construct(bufer + i, _data_[i]);
			_alloc_.destroy(_data_ + i);
		}
		
		_alloc_.construct(_data_ + _size_++, elem);
		_alloc_.deallocate(_data_, _maxsize_);
		_maxsize_ *= 2;
		_data_ = bufer;
	}
	
	friend std::ostream& operator << (std::ostream& out, const Vector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
		{
			out << other[i] << ' ';
		}
		return out;
	}
	
	value_type pop_back()
	{
		if(_size_)
		{
			value_type bufer = _data_[_size_-- - 1];
			_alloc_.destroy(_data_ + _size_);
			return bufer;
		}
		throw std::out_of_range("Vector is empty");
	}
	
	size_type capacity() const
	{
		return _maxsize_;
	}
	
	void reserve(size_type count)
	{
		if (_maxsize_ != count)
		{
			pointer bufer = _alloc_.allocate(count);

			if (_size_ > count)
				_size_ = count;
				
			for (size_type i = 0; i < _size_; ++i)
			{
				_alloc_.construct(bufer + i, _data_[i]);
				_alloc_.destroy(_data_ + i);
			}
			_alloc_.deallocate(_data_, _maxsize_);

			_data_ = bufer;
			_maxsize_ = count;
		}
	}
	
	void resize(size_type count)
	{
		if (_size_ > count)
			for (size_type i = count; i < _size_; ++i)
				_alloc_.destroy(_data_ + i);
		else
		{
			if (_maxsize_ < count)
				reserve(count);


			for (size_type i = _size_; i < count; ++i)
			{
				_alloc_.construct(_data_ + i);
			}
		}
		_size_ = count;
	}
	
	iterator begin() const noexcept
	{
		return iterator(_data_);
	}

	iterator end() const noexcept
	{
		return iterator(_data_ + _size_);
	}
	
	reverse_iterator rbegin() const noexcept
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend() const noexcept
	{
		return reverse_iterator(begin());
	}
// ------------------
	bool operator == (const Vector& other) const
	{
		if(_size_ != other._size_)
			return false;
		for(size_t i = 0; i < _size_; ++i)
			if(_data_[i] != other._data_[i])
				return false;
		return true;
	}
	
	bool operator != (const Vector& other) const
	{
		return !this->operator == (other);
	}
	
	bool operator > (const Vector& other) const
	{
		if(_size_ > other._size_)
			return true;
		if(_size_ < other._size_)
			return false;
		for(size_t i = 0; i < _size_; ++i)
			if(_data_[i] <= other._data_[i])
				return false;
		return true;
	}
	
	bool operator < (const Vector& other) const
	{
		if(_size_ < other._size_)
			return true;
		if(_size_ > other._size_)
			return false;
		for(size_t i = 0; i < _size_; ++i)
			if(_data_[i] >= other._data_[i])
				return false;
		return true;
	}
	
	bool operator >= (const Vector& other) const
	{
		if(_size_ > other._size_)
			return true;
		if(_size_ < other._size_)
			return false;
		for(size_t i = 0; i < _size_; ++i)
			if(_data_[i] < other._data_[i])
				return false;
		return true;
	}
	
	bool operator <= (const Vector& other) const
	{
		if(_size_ < other._size_)
			return true;
		if(_size_ > other._size_)
			return false;
		for(size_t i = 0; i < _size_; ++i)
			if(_data_[i] > other._data_[i])
				return false;
		return true;
	}
};


