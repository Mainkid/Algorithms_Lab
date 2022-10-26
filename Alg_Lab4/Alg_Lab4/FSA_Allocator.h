#pragma once
#include "windows.h"
#define MEMSIZE 4096

//Убрать циклы, сделать за константное время!!!
//Помудрить с приватностью
//Добавить дебаг методы
//Избавиться от утечек памяти

typedef void* voidPtr;


class MemoryPageFSA
{
public:
	MemoryPageFSA* next;
	void* pagePtr;
	void** freeList;
	size_t blocks;
	size_t blockSize;
	int numInitialized;


	MemoryPageFSA()
	{

	}


	MemoryPageFSA(size_t blocks,size_t blockSize,void* pagePtr)
	{
		this->blocks = blocks;
		this->blockSize = blockSize;
		freeList = new void*[blocks];
		next = nullptr;
		numInitialized = 0;
		this->pagePtr = pagePtr;

		for (int i = 0; i < blocks; i++)
		{
			freeList[i] = (char*)pagePtr + i * blockSize;
		}

	}

	MemoryPageFSA(size_t blocks, size_t blockSize)
	{
		this->blocks = blocks;
		this->blockSize = blockSize;
		freeList = new void* [blocks];
		next = nullptr;
		numInitialized = 0;
		this->pagePtr = VirtualAlloc(nullptr, MEMSIZE, MEM_COMMIT, PAGE_READWRITE);

		for (int i = 0; i < blocks; i++)
		{
			freeList[i] = (char*)pagePtr + i * blockSize;
		}
	}
	
	~MemoryPageFSA()
	{
		delete[] freeList;
	}

	MemoryPageFSA* createPage(size_t blocks, size_t block_size)
	{
		void* p = nullptr;
		p = VirtualAlloc(nullptr, MEMSIZE, MEM_COMMIT, PAGE_READWRITE);
		MemoryPageFSA newPage(blocks, block_size, p);
		return& newPage;
	}

	void* alloc()
	{
		if (numInitialized == blocks)
		{
			if (!this->next)
			{
				next = new MemoryPageFSA(blocks, blockSize);
			}
			return this->next->alloc();
			
		}
		else
		{

			numInitialized += 1;
			return freeList[numInitialized - 1];


		}
	}

	bool free(void* p)
	{
		if ((char*)p - (char*)pagePtr < MEMSIZE)
		{
			if (((char*)p - (char*)pagePtr) % blockSize != 0)
				return false;
			else
			{
				numInitialized -= 1;
				
				int index = (((char*)p - (char*)pagePtr) / blockSize);
				freeList[index] = freeList[numInitialized];
				freeList[numInitialized] =p;
				return true;
			}
		}
		else if (next != nullptr)
		{
			return next->free(p);
		}
		else return false;
	}

	void dumpBlocks(int pageNum)
	{
		for (int i = 0; i < numInitialized; i++)
		{
			std::cout<< "Page: " << pageNum << ", " << "address: " << freeList[i] << std::endl;
		}

		if (next != nullptr)
			next->dumpBlocks(pageNum + 1);
	}

};


class FSA
{
public:

	FSA()
	{

	}

	~FSA()
	{
		while (startPage != nullptr)
		{
			MemoryPageFSA* next = startPage->next;
			VirtualFree(startPage, 0, MEM_RELEASE);
			delete(startPage);
			startPage = next;
		}
	}

	void init(size_t block_size)
	{
		startPage = new MemoryPageFSA(MEMSIZE / block_size, block_size);
	}

	MemoryPageFSA createPage(size_t blocks,size_t block_size)
	{

		MemoryPageFSA newPage(blocks, block_size);
		return newPage;
	}

	void destroy()
	{
		//Освободить память, выделенную под массив булов
	/*	do
		{
			VirtualFree(startPage->pagePtr, 0, MEM_RELEASE);
		} while (startPage->next != nullptr);*/
	}

	void* alloc()
	{
		return startPage->alloc();
	}

	bool free(void* p)
	{
		return startPage->free(p);
	}

	void dumpBlocks()
	{
		startPage->dumpBlocks(1);
	}
	

private:
	MemoryPageFSA* startPage;
};