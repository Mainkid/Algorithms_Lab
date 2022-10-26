#pragma once

constexpr int defaultCapacity = 4;

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
			iteratorPos = 0;
		}

		Iterator(int* curSize, T* arr)
		{
			iteratorPos = 0;
			this->size = curSize;
			this->arr = arr;
			
		}

		~Iterator()
		{

		}

		const T& get() const
		{
			return *(arr+iteratorPos);
		}

		void set(const T& value)
		{
			*(arr+iteratorPos) = value;
		}
		void next()
		{
			iteratorPos += 1;
		}
		bool hasNext()
		{
			if (iteratorPos < *size)
			{
				return true;
			}
			else
				return false;
		}
	private:
		int iteratorPos;
		int* size;
		T* arr;

	};

	Iterator<T> iterator()
	{
		Iterator<T> it(currentSize, (*arrayPtr));
		return it;
	}


	Array()
	{
		
		arrayPtr = (T**)malloc(sizeof(T*));
		(*arrayPtr) = (T*)malloc(sizeof(T) * defaultCapacity);
		maxSize = (int*)malloc(sizeof(int));
		currentSize = (int*)malloc(sizeof(int));
		referenceAmount = (int*)malloc(sizeof(int));
		*maxSize = defaultCapacity;
		*currentSize = 0;
		*referenceAmount = 1;


	}

	Array(int capacity)
	{

		arrayPtr = (T**)malloc(sizeof(T*));
		(*arrayPtr) = (T*)malloc(sizeof(T) * defaultCapacity);
		maxSize = (int*)malloc(sizeof(int));
		currentSize = (int*)malloc(sizeof(int));
		referenceAmount= (int*)malloc(sizeof(int));
		*maxSize = capacity;
		*currentSize = 0;
		*referenceAmount = 1;

		
	}

	~Array()
	{
		if (*referenceAmount > 1)
		{
			*referenceAmount -= 1;
		}
		else
		{
			free(*arrayPtr);
			free(maxSize);
			free(currentSize);
			free(referenceAmount);
		}


	}

	Array& operator=(const Array& other)
	{
		if (this != &other)
		{
			this->clear();
			this->arrayPtr = (other.arrayPtr);
			this->referenceAmount = (other.referenceAmount);
			this->maxSize = (other.maxSize);
			this->currentSize = (other.currentSize);
			*(this->referenceAmount) += 1;
		}

		return *this;
	}


	int insert(const T& value)
	{
		*currentSize += 1;

		if (*currentSize > *maxSize)
			expandArray();
		(*arrayPtr)[*currentSize - 1] = value;
		return 0;
	}
	//Добавить move-семантику!!!

	int insert(int index, const T& value)
	{
		if (index > *currentSize)
		{
			insert(value);
			return 0;
		}

		*currentSize += 1;
		if (( *currentSize) > *maxSize)
			expandArray();

		for (int i = *currentSize; i > index; i--)
			(*arrayPtr)[i] =(*arrayPtr)[i - 1];

		(*arrayPtr)[index] = value;

		return 0;
	}

	void  remove(int index)
	{
		if (index >= *currentSize)
			return;

		*currentSize -= 1;
		for (int i = 0; i < *currentSize; i++)
			(*arrayPtr)[i] = (*arrayPtr)[i + 1];
	}


	const T& operator[] (int index) const
	{
		return (*arrayPtr)[index];
	}

	T& operator [](int index)
	{
		return (*arrayPtr)[index];
	}

	int size() const
	{
		return *currentSize;
	}

	

	

	

private:

	

	void expandArray()
	{
		T* tmp = (T*)malloc(sizeof(T) * (*maxSize) * 2);
		for (int i = 0; i < *maxSize; i++)
			tmp[i] = (*arrayPtr)[i];



		*arrayPtr = tmp;
		*maxSize *= 2;
	}

	void clear()
	{
		free(*arrayPtr);
	}

	
	int* maxSize;
	int* currentSize;
	int* referenceAmount;
	T** arrayPtr;
};