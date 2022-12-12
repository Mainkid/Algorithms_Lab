#pragma once
#include "windows.h"
#include <cassert> 
#define BLOCKS 256 //Фиксированное количество блоков для каждого типа FSA
#define DEBUG (1)
//Убрать циклы, сделать за константное время!!!
//Помудрить с приватностью
//Добавить дебаг методы
//Избавиться от утечек памяти

//Доделать assert
//Доделать DumpBlocks

typedef void* voidPtr;

struct Header
{
	Header* next;
	void* 
};

class MemoryPageFSA
{
public:
	MemoryPageFSA* next;
	void* pagePtr;
	int* freeList;
	int* fHead;
	size_t blocks;
	size_t blockSize;
	int numInitialized;


	MemoryPageFSA()
	{

	}

	MemoryPageFSA(size_t blockSize,void* pagePtr)
	{
		this->blocks = BLOCKS;
		this->blockSize = blockSize;
		freeList = new int [blocks];
		next = nullptr;
		numInitialized = 0;
		this->pagePtr = pagePtr;

		for (int i = 0; i < blocks - 1; i++)
		{
			freeList[i] = i + 1;
		}
		freeList[blocks - 1] = -2;

		fHead = freeList;

	}

	MemoryPageFSA(size_t blockSize)
	{
		this->blocks = BLOCKS;
		this->blockSize = blockSize;
		freeList = new int [blocks];
		next = nullptr;
		numInitialized = 0;
		this->pagePtr = VirtualAlloc(nullptr, blockSize*BLOCKS, MEM_COMMIT, PAGE_READWRITE);

		for (int i = 0; i < blocks-1; i++)
		{
			freeList[i] = i + 1;
		}
		freeList[blocks - 1] = -2;

		fHead = freeList;
	}
	
	~MemoryPageFSA()
	{
		delete[] freeList;
	}

	MemoryPageFSA* createPage(size_t block_size)
	{
		void* p = nullptr;
		p = VirtualAlloc(nullptr, block_size*BLOCKS, MEM_COMMIT, PAGE_READWRITE);
		MemoryPageFSA newPage(block_size, p);
		return &newPage;
	}

	void* alloc()
	{

		if (numInitialized == blocks)
		{
			if (!this->next)
			{
				next = new MemoryPageFSA(blockSize);
			}
			return this->next->alloc();
			
		}
		else
		{
			numInitialized += 1;
			int tmp;
			int* prevHead=fHead;
			
			if (numInitialized < BLOCKS)
			{
				tmp = *fHead;
				prevHead = fHead;
				*fHead = -1;
				fHead = freeList + tmp;
			}
			else
			{
				fHead = nullptr;
			}
			

			void* ll = ((char*)pagePtr + ((prevHead - freeList) * blockSize));
			return ((char*)pagePtr + ((prevHead - freeList) * blockSize));
		}
	}

	bool free(void* p)
	{
		if ((char*)p - (char*)pagePtr < BLOCKS*blockSize)
		{
			if (((char*)p - (char*)pagePtr) % blockSize != 0)
			{
				std::cout <<p<< " Incorrect address!" << std::endl;
#ifdef DEBUG
				assert(false&&"Incorrect address!");
#endif
				return false;

			}
				
			else
			{
				
				numInitialized -= 1;

				int index = (((char*)p - (char*)pagePtr) / blockSize);

				if (numInitialized < BLOCKS - 1)
					freeList[index] = fHead - freeList;
				else
					freeList[index] = -2;
				fHead = freeList + index;
				
				return true;
			}
		}
		else if (next != nullptr)
		{
			return next->free(p);
		}
		else
		{
			std::cout <<p<< " Incorrect address!" << std::endl;
#ifdef DEBUG
			assert(false&&"Incorrect address!");
#endif
			return false;
		}
	}

#ifdef DEBUG
	void dumpBlocks(int pageNum) const
	{
		std::cout << "FSA" << blockSize << " allocator:" << std::endl;
		for (int i = 0; i <BLOCKS; i++)
		{
			if (freeList[i]==-1)
				std::cout<< "Page: " << pageNum << ", " << "address: " << (void*)((char*)pagePtr+(i*blockSize)) << std::endl;
		}

		if (next != nullptr)
			next->dumpBlocks(pageNum + 1);
	}

	void assertLeaks()
	{
		for (int i = 0; i < BLOCKS; i++)
		{
			if (freeList[i] == -1)
				assert(freeList[i] != -1&&"FSA MEMORY LEAK FOUND");
		}
		if (next != nullptr)
			next->assertLeaks();
	}

#endif

};

class FSA
{
public:

	FSA() {
		isInitialized = false;
	}

	~FSA()
	{
#ifdef DEBUG
		assert(!isInitialized && "FSA should be deinitialized before destructor call!");
#endif
		//destroy();
	}

	void init(size_t block_size)
	{
#ifdef DEBUG
		assert(!isInitialized && "FSA is already initialized!");
#endif
		startPage = new MemoryPageFSA(block_size);
		isInitialized = true;
	}

	MemoryPageFSA createPage(size_t blocks,size_t block_size)
	{

		MemoryPageFSA newPage(block_size);
		return newPage;
	}

	void destroy()
	{
#ifdef DEBUG
		assert(isInitialized && "FSA should be initialized!");
		startPage->assertLeaks();
#endif
		while (startPage != nullptr)
		{
			MemoryPageFSA* next = startPage->next;
			VirtualFree(startPage, 0, MEM_RELEASE);
			delete(startPage);
			startPage = next;
		}

		isInitialized = false;
	}


	void* alloc()
	{
#ifdef DEBUG
		assert(isInitialized && "FSA should be initialized!");
#endif
		return startPage->alloc();
	}


	bool free(void* p)
	{
#ifdef DEBUG
		assert(isInitialized && "FSA should be initialized!");
#endif
		return startPage->free(p);
	}

#ifdef DEBUG
	void dumpBlocks() const
	{
		assert(isInitialized && "FSA should be initialized!");

		startPage->dumpBlocks(1);
	}
#endif

private:
	MemoryPageFSA* startPage;
	bool isInitialized;
	int memorySize;
};