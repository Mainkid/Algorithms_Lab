#pragma once
#include <cassert> 

#define CoalescePageSize 20480
#define DEBUG (1)

struct dataBlock
{
public:
	struct dataBlock* next;
	struct dataBlock* prev;
	int size;
	bool inUse;

};

struct MemoryPageCoalesce
{
public:
	MemoryPageCoalesce* next;
	void* pagePtr;
	struct dataBlock* head;

	MemoryPageCoalesce(size_t pageSize)
	{
		
		pagePtr= VirtualAlloc(nullptr, CoalescePageSize, MEM_COMMIT, PAGE_READWRITE);
		this->head = (struct dataBlock*)(pagePtr);
		
		next = nullptr;
		head->size= CoalescePageSize - sizeof(struct dataBlock);
		

	}

	void* alloc(size_t size)
	{
		struct dataBlock* block = findFirstFit(size);
		
		if (block == nullptr)
		{
			MemoryPageCoalesce* iter = this;
			while (iter->next != nullptr)
				iter = iter->next;
			iter->next = new MemoryPageCoalesce(CoalescePageSize);
			return iter->next->alloc(size);
		}

		else
		{
			size_t oldSize = block->size;
			block->inUse = true;
			block->size = size;
			dataBlock* oldNext = block->next;
			if (oldNext != ((dataBlock*)((char*)block + block->size)))
			{
				block->next = (dataBlock*)((char*)block + block->size +sizeof(struct dataBlock));
				((struct dataBlock*)((char*)block + block->size + sizeof(struct dataBlock)))->prev = block;
				((struct dataBlock*)((char*)block + block->size + sizeof(struct dataBlock)))->next = oldNext;
				((struct dataBlock*)((char*)block + block->size + sizeof(struct dataBlock)))->inUse = false;
				((struct dataBlock*)((char*)block + block->size + sizeof(struct dataBlock)))->size = oldSize - size - sizeof(struct dataBlock);
			}


			return (void*)((char*)block + sizeof(struct dataBlock));
		}

	}

	bool free(void* p)
	{

		if ((char*)p - (char*)pagePtr < CoalescePageSize)
		{
#ifdef DEBUG
			assert(isCorrectPtr(p)&&"Incorrect address!");
#endif
			
			((struct dataBlock*)((char*)p - sizeof(struct dataBlock)))->inUse = false;
			tryCoalesce(((struct dataBlock*)((char*)p - sizeof(struct dataBlock))));
		}
		else if (next != nullptr)
		{
			return next->free(p);
		}
		else
		{
#ifdef DEBUG
			assert(isCorrectPtr(p) && "Incorrect address!");
#endif
			return false;
		}
		
	}
	
	void tryCoalesce(dataBlock* structPtr)
	{
		while (structPtr->prev != nullptr && structPtr->prev->inUse == false)
		{
			size_t size = structPtr->size;
			if (structPtr->next != nullptr)
			{
				structPtr->next->prev = structPtr->prev;
			}
			structPtr->prev->next = structPtr->next;
			structPtr->prev->size += size + sizeof(struct dataBlock);
			structPtr = structPtr->prev;
		}

		while (structPtr->next != nullptr && structPtr->next->inUse == false)
		{
			
			size_t size = structPtr->next->size;
			if (structPtr->next->next != nullptr)
			{
				structPtr->next->next->prev = structPtr;
				
			}
			structPtr->next = structPtr->next->next;
			structPtr->size += size + sizeof(struct dataBlock);
			
		}
	}

	dataBlock* findFirstFit(size_t size)
	{
		dataBlock* iterator;
		MemoryPageCoalesce* iteratorPage=this;
		while (iteratorPage != nullptr)
		{
			iterator = iteratorPage->head;
			while (iterator != nullptr)
			{
				if (iterator->inUse == false && iterator->size >= size + sizeof(struct dataBlock))
					return iterator;
				iterator = iterator->next;
			}
			iteratorPage = iteratorPage->next;
		}
		return nullptr;

	}


	bool isCorrectPtr(void* p)
	{
		dataBlock* iterHead = head;
		while (iterHead != nullptr)
		{
			if ((void*)((char*)iterHead + sizeof(struct dataBlock)) == p)
				return true;
			iterHead = iterHead->next;
		}
		return false;
	}
#ifdef DEBUG
	void assertLeaks()
	{
		dataBlock* iter = head;
		while (iter != nullptr)
		{
			if (iter->inUse)
				assert(!iter->inUse && "COALESCE MEMORY LEAK FOUND");
			iter = iter->next;

		}
		if (next != nullptr)
			next->assertLeaks();
	}

	void dumpBlocks(int pageNum) const
	{
		

		dataBlock* iterHead = head;
		while (iterHead != nullptr)
		{
			if (iterHead->inUse)
			{
				std::cout << "Page: " << pageNum << ", " << "address: " << (void*)((char*)iterHead + sizeof(struct dataBlock)) << std::endl;
			}
			iterHead = iterHead->next;
		}
		if (next != nullptr)
			next->dumpBlocks(pageNum + 1);
	}

#endif
};


class Coalesce
{
public:
	Coalesce()
	{
		isInitialized = false;
	}

	~Coalesce()
	{
#ifdef DEBUG
		assert(!isInitialized && "Coalesce should be deinitialized before destructor call!");
#endif
	}

	void init()
	{
#ifdef DEBUG
		assert(!isInitialized && "Coalesce is already initialized!");
#endif
		startPage = new MemoryPageCoalesce(CoalescePageSize);
		isInitialized = true;
	}

	void* alloc(size_t size)
	{
#ifdef DEBUG
		assert(isInitialized && "Coalesce should be initialized!");
#endif
		size = allign(size);
		return startPage->alloc(size);
	}

	bool free(void* ptr)
	{
#ifdef DEBUG
		assert(isInitialized && "Coalesce should be initialized!");
#endif
		return startPage->free(ptr);
	}

	void destroy()
	{
#ifdef DEBUG
		assert(isInitialized && "Coalesce should be initialized!");
		startPage->assertLeaks();
#endif
		while (startPage != nullptr)
		{
			MemoryPageCoalesce* next = startPage->next;
			VirtualFree(startPage, 0, MEM_RELEASE);
			delete(startPage);
			startPage = next;
		}

		isInitialized = false;
	}

#ifdef DEBUG
	void dumpBlocks() const
	{
		assert(isInitialized && "Coalesce should be initialized!");
		std::cout << "Coalesce allocator:" << std::endl;
		startPage->dumpBlocks(1);
	}
#endif

private:
	MemoryPageCoalesce* startPage;
	bool isInitialized;
	size_t allign(size_t size)
	{
		if (size % 8 == 0)
			return size;
		else
			return 8 * (size / 8 + 1);
	}
};