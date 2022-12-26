#pragma once

constexpr int DEFAULT_CAPACITY = 4;

template <typename T>
class Array final
{
public:
	
	
	template <typename T>
	class Iterator
	{
	public:

		Iterator()
		{
			
		}

		Iterator(T* ptr,Array* arr,int dir)
		{
			this->ptr = ptr;
			this->dir = dir;
			this->arr = arr;
		}

		~Iterator()
		{

		}

		const T& get() const
		{
			return *ptr;
		}

		void set(const T& value)
		{
			ptr->~T();
			new (ptr) T(value);
		}
		void next()
		{
			ptr += dir;
		}
		bool isLast() const
		{
			return (ptr + dir >= arr->arrayPtr && ptr + dir <= arr->arrayPtr + arr->size() - 1);
		}
	private:
		int dir;
		T* ptr;
		Array* arr;

	};

	template <typename T>
	class ConstIterator
	{
		ConstIterator()
		{

		}

		ConstIterator(T* ptr, Array* arr, int dir)
		{
			this->ptr = ptr;
			this->dir = dir;
			this->arr = arr;
		}

		~ConstIterator()
		{

		}

		const T& get() const
		{
			return *ptr;
		}
		void next()
		{
			ptr += dir;
		}
		bool isLast() const
		{
			return (ptr + dir >= arr->arrayPtr && ptr + dir <= arr->arrayPtr + arr->size() - 1);
		}
	private:
		int dir;
		T* ptr;
		Array* arr;
	};

	Iterator<T> iterator()
	{
		Iterator<T> it(arrayPtr, this, 1);
		return it;
	}

	Iterator<T> reverseIterator()
	{
		Iterator<T> it(arrayPtr+currentSize-1, this, -1);
		return it;
	}

	ConstIterator<T> iterator() const
	{
		ConstIterator<T> it(arrayPtr, this, 1);
		return it;
	}

	ConstIterator<T> reverseIterator() const
	{
		ConstIterator<T> it(arrayPtr + currentSize - 1, this, -1);
		return it;
	}

	Array()
	{
		
		arrayPtr = (T*)malloc(sizeof(T) * DEFAULT_CAPACITY);
		maxSize = DEFAULT_CAPACITY;
		currentSize = 0;
	}

	Array(int capacity)
	{
		arrayPtr = (T*)malloc(sizeof(T) * capacity);
		maxSize = capacity;
		currentSize = 0;
	}

	Array(Array&& other)
	{

		arrayPtr = other.arrayPtr;
		size = other.size;
		maxSize = other.maxSize;

		other.arrayPtr = nullptr;
		other.size = 0;
		other.capacity = 0;

	}

	~Array()
	{
		clear();
	}

	Array& operator=(const Array& other)
	{
		Array temp(other);
		swap(temp);	
		return *this;
	}

	 Array& operator=(Array&& other) noexcept
	{
		if (this != &other)
		{
			
			for (int i = 0; i < currentSize; i++) {
				arrayPtr[i].~T();
			}
			free(arrayPtr);

			currentSize = other.currentSize;
			arrayPtr = other.arrayPtr;
			maxSize = other.maxSize;

			other.maxSize = 0;
			other.arrayPtr = nullptr;
			other.currentSize = 0;

		}
		return *this;
	}

	Array(const Array& other)
	{

		arrayPtr =(T*) malloc(other.size() * sizeof(T));
		for (int i = 0; i < other.size(); i++)
		{
			new (arrayPtr + i)T(other.arrayPtr[i]);
		}
		currentSize = other.size();
		maxSize = other.maxSize;

	}

	int insert(const T& value)
	{
		return insert(currentSize, value);
	}
	//ћб улучшить производительность??
	int insert(int index, const T& value)
	{
		if (index > currentSize)
		{
			return -1;
		}

		
		if (currentSize == maxSize)
		{
			expandArray();
		}

		new (arrayPtr + currentSize) T(std::move(arrayPtr[currentSize-1]));

		for (int i = currentSize-1; i > index; i--)
			arrayPtr[i]=(std::move(arrayPtr[i - 1]));

		arrayPtr[index] = value;
		
		currentSize += 1;
		return index;
	}

	void  remove(int index)
	{
		if (index >= currentSize || index<0)
			return;

		currentSize -= 1;

		for (int i = index; i < currentSize; i++)
			arrayPtr[i]=T(std::move(arrayPtr[i + 1]));

		(&arrayPtr[currentSize])->~T();
	}


	const T& operator[] (int index) const
	{
		return arrayPtr[index];
	}

	T& operator [](int index)
	{
		return arrayPtr[index];
	}

	int size() const
	{
		return currentSize;
	}

	

	

	

private:

	
	//!!!
	void expandArray()
	{
		T* tmp = (T*)malloc(sizeof(T) * (maxSize) * 2);
		for (int i = 0; i < maxSize; i++)
			new (tmp+ i)T(std::move(arrayPtr[i]));

		clear();

		arrayPtr = tmp;
		maxSize *= 2;
	}

	void clear()
	{
		for (int i = 0; i < currentSize; i++) {
			(arrayPtr[i]).~T();
		}
		free(arrayPtr);
	}

	void swap(Array& other)
	{
		int tmp;
		T* tmpT;

		tmp = this->maxSize;
		this->maxSize = other.maxSize;
		other.maxSize = tmp;

		tmp = this->currentSize;
		this->currentSize = other.currentSize;
		other.currentSize = tmp;

		tmpT = this->arrayPtr;
		this->arrayPtr = other.arrayPtr;
		other.arrayPtr = tmpT;
	}

	
	int maxSize;
	int currentSize;
	T* arrayPtr;
};