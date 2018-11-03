/* Created by Michael Pritugin */

#include <iostream>
#include <cstdlib>


/* 
 * What i need in MyVector:
 * 1) MyVector(...) + test
 * 2) ~MyVector() + test
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
 * 19) MyVector(const MyVector& other) +
 * 20) MyVector(const MyVector&& other) +
 * 21) std::cout << + test
*/

template <class T>
class MyVector
{
private:
	T* _data_;
	size_t _size_;
	size_t _maxsize_;
public:
	// Standart constructor
	MyVector(size_t size, T* data = nullptr) : _size_(size), _maxsize_(size + 4)
	{
		_data_ = (T*) calloc(_maxsize_, sizeof(T));
		if(data)
			std::copy(data, data + size, _data_);
	}
	
	// Empty constuctor
	MyVector() : _size_(0), _maxsize_(4)
	{
		_data_ =  (T*) calloc(_maxsize_, sizeof(T));
	}
	
	// Copy constructors
	MyVector(const MyVector& other) : MyVector(other._size_, other._data_) {}
	
	// Move constructors
	MyVector(MyVector&& other) : _size_(other._size_),
	_maxsize_(other._maxsize_), _data_(other._data_) 
	{
		other._size_ = 0;
		other._maxsize_ = 0;
        other._data_ = nullptr;
	}
	
	// Standart destructor
	~MyVector()
	{
		free(_data_);
	}
	
	// operator =
	MyVector& operator = (const MyVector& other)
	{
		_size_ = other._size_;
		_maxsize_ = other._maxsize_;
		_data_ = (T*) realloc(_data_, _size_ * sizeof(T)); 
		std::copy(other._data_, other._data_ + _size_, _data_);
        return *this;
	}
	
	size_t size() const { return _size_; }
	
	// operator []
	T& operator [](size_t pos)
	{
		if (pos < _size_)
			return _data_[pos];
		throw std::out_of_range("Wrong index");
	}
	
	const T operator [](size_t pos) const
	{
		if (pos < _size_)
			return _data_[pos];
		throw std::out_of_range("Wrong index");
	}
	
	const T front() const
	{
		if(_size_)
			return _data_[0];
		throw std::out_of_range("Vector is empty");
	}
	
	const T back() const
	{
		if(_size_)
			return _data_[_size_ - 1];
		throw std::out_of_range("Vector is empty");
	}
	
	bool empty() const
	{
		return _size_ == 0;
	}
	
	void push_back(const T& elem)
	{
		if(_size_ < _maxsize_)
		{
			_data_[_size_++] = elem;
			return;
		}
		
		_maxsize_ += 4;
		
		_data_ = (T*) realloc(_data_, _maxsize_ * sizeof(T));
		
		_data_[_size_++] = elem;
	}
	
	friend std::ostream& operator << (std::ostream& out, const MyVector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
		{
			out << other[i] << ' ';
		}
		return out;
	}
	
	T pop_back()
	{
		if(_size_)
			return _data_[_size_-- - 1];
		throw std::out_of_range("Vector is empty");
	}
	
	void reserve(size_t count)
	{
		_maxsize_ = count + 4;
		_size_ = count;
		_data_ = (T*) realloc(_data_, _maxsize_ * sizeof(T));
	}
	
	void resize(size_t count)
	{
		_maxsize_ = count + 4 + 1;
		_size_ = count+1;
		_data_ = (T*) realloc(_data_, _maxsize_ * sizeof(T));
	}
	
	bool operator == (const MyVector& other) const
	{
		if(_size_ != other._size_)
			return false;
		for(size_t i = 0; i < _size_; ++i)
			if(_data_[i] != other._data_[i])
				return false;
		return true;
	}
	
	bool operator != (const MyVector& other) const
	{
		return !this->operator == (other);
	}
	
	bool operator > (const MyVector& other) const
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
	
	bool operator < (const MyVector& other) const
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
	
	bool operator >= (const MyVector& other) const
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
	
	bool operator <= (const MyVector& other) const
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


