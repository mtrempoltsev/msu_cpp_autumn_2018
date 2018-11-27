#include <iostream>
#include <iterator>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

template <class T>
class Allocator
{
public:
	
	using value_type = T;
	using pointer = T*;
	using size_type = size_t;
    
	pointer allocate(size_type n)
	{
		return static_cast<pointer>(::operator new(n * sizeof(value_type)));
	}
	
	void deallocate(pointer p, size_type count)
	{
		::operator delete[](p, sizeof(value_type) * count);
	}
	
	template<class... Args >
	void construct(pointer p, Args&&... args)
	{
		::new(p) T(std::forward<Args>(args)...);
	}
	
	void destroy(pointer p)
	{
		p->~value_type();
	}		
};

template <class T>
class Iterator
	: public std::iterator<std::random_access_iterator_tag, T>
{
    T* ptr_;
    
public:
    
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;
    
    explicit Iterator(pointer ptr)
        : ptr_(ptr)
    {}

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

    Iterator& operator++()
    {
        ++ptr_;
        return *this;
    }
    
    Iterator& operator+=(const size_type& n)
    {
		for (size_type i = 0; i < n; ++i)
		{
			++ptr_;
		}
        return *this;
    }    
    
    Iterator& operator--()
    {
        --ptr_;
        return *this;
    } 
    
    Iterator& operator-=(const size_type& n)
    {
		for (size_type i = 0; i < n; ++i)
		{
			--ptr_;
		}
        return *this;
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
    using rvalueref = T&&;
    using const_reference = const T&;
    using const_rvalueref = const T&&;
	using allocator_type = Alloc;
	using iterator = Iterator<T>;
	using reverse_iterator = std::reverse_iterator<Iterator<T>>;
	
private:
    Alloc alloc_;
    pointer data_;
    size_type size_;
    size_type max_size_;   
    
public:
    explicit Vector(size_type count) : size_(count), max_size_(count * 2)
    {
		data_ = alloc_.allocate(max_size_);
		for (size_type i = 0; i < size_; ++i)
		{
			alloc_.construct(data_ + i);
		}
	}
	
	explicit Vector() : size_(0), max_size_(8)
    {
		data_ = alloc_.allocate(max_size_);
	}
	
	~ Vector()
	{
		clear();
		alloc_.deallocate(data_, max_size_);
	}

	void clear() noexcept
	{
		for (size_type i = 0; i < size_; ++i)
		{
			alloc_.destroy(data_ + i);
		}
		size_ = 0;
		max_size_ = 0;
	}
	
	bool empty() const
	{
		return size_ == 0;
	}
	
	size_type size() const
	{
		return size_;
	}
	
	void push_back(const_reference value)
	{
		if (size_ >= max_size_)
		{
			size_type new_max_size_ = (max_size_ > 0) ? (2 * max_size_) : 8;
			reserve(new_max_size_);
		}
		alloc_.construct(data_ + size_++, value);
	}	
	
	void push_back(rvalueref value)
	{
		if (size_ >= max_size_)
		{
			size_type new_max_size_ = (max_size_ > 0) ? (2 * max_size_) : 8;
			reserve(new_max_size_);
		}
		alloc_.construct(data_ + size_++, forward<value_type>(value));
	}
	
	reference operator[](size_type pos)
	{
		if (pos < size_)
		{
			return data_[pos];
		}
		else
		{
			string errorMessage = string("Position ") + to_string(pos); 
			errorMessage += string(" is out of bounds. Current size is ");
			errorMessage += to_string(size_);
			throw std::out_of_range(errorMessage);
		}
	}
	
	void pop_back()
	{
		if (size_ > 0)
		{
			alloc_.destroy(data_ + size_);
			--size_;
		}
		else
		{
			throw std::out_of_range("Can't throw out element from empty array");
		}			
	}
	
	void reserve(size_type size)
	{
		if (max_size_ < size)
		{
			size_type new_max_size_ = size;
			pointer newData = alloc_.allocate(new_max_size_);
			for (size_type i = 0; i < size_; ++i)
			{
				alloc_.construct(newData + i, forward<value_type>(*(data_ + i)));
				alloc_.destroy(data_ + i);
			}
			alloc_.deallocate(data_, max_size_);
			max_size_ = new_max_size_;
			data_ = newData;			
		}	
	}
	
	size_type capacity() const
	{
		return max_size_;
	}	 
		
	void resize(size_type count)
	{
		if (max_size_ != count)
		{
			size_type new_max_size = (max_size_ < count) ? count : max_size_;
			size_type i_max = (size_ < count) ? size_ : count;
			pointer newData = alloc_.allocate(new_max_size);			
			
			for (size_type i = 0; i < i_max; ++i)
			{
				alloc_.construct(newData + i, forward<value_type>(*(data_ + i)));
				alloc_.destroy(data_ + i);				
			}
			
			for (size_type i = i_max; i < size_; ++i)
			{
				alloc_.destroy(data_ + i);
			}
			
			alloc_.deallocate(data_, max_size_);
			data_ = newData;
			
			for (size_type i = i_max; i < count; ++i)
			{
				alloc_.construct(data_ + i);
			}
						
			size_ = count;
			max_size_ = new_max_size;
		}	
	}
	
	iterator begin() const noexcept
	{
		return iterator(data_);
	}

	iterator end() const noexcept
	{
		return iterator(data_ + size_);
	}
	
	reverse_iterator rbegin() const noexcept
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend() const noexcept
	{
		return reverse_iterator(begin());
	} 
};
