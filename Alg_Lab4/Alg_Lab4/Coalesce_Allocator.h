#pragma once

#define CoalescePageSize 20480

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
		
		//assert(head != 0);
		next = nullptr;
		head->size= CoalescePageSize - sizeof(struct dataBlock);
		

	}

	void* alloc(size_t size)
	{
		struct dataBlock* block = findFirstFit(size);
		
		if (block == nullptr)
		{
			next = new MemoryPageCoalesce(CoalescePageSize);
			return next->alloc(size);
		}
		else
		{
			size_t oldSize = block->size;
			block->inUse = true;
			block->size = size;
			dataBlock* oldNext = block->next;
			if (oldNext != ((dataBlock*)((char*)block + block->size)))
			{
				block->next = (dataBlock*)((char*)block + block->size);
				((struct dataBlock*)((char*)block + block->size))->prev = block;
				((struct dataBlock*)((char*)block + block->size))->next = oldNext;
				((struct dataBlock*)((char*)block + block->size))->inUse = false;
				((struct dataBlock*)((char*)block + block->size))->size = oldSize - size - sizeof(struct dataBlock);
			}
			//int a = sizeof(struct dataBlock);
			//void* b = (void*)((char*)block + sizeof(struct dataBlock));
			return (void*)((char*)block + sizeof(struct dataBlock));
		}

	}

	bool free(void* p)
	{

		if ((char*)p - (char*)pagePtr < CoalescePageSize)
		{
			((struct dataBlock*)((char*)p - sizeof(struct dataBlock)))->inUse = false;
			tryCoalesce(((struct dataBlock*)((char*)p - sizeof(struct dataBlock))));
		}
		else if (next != nullptr)
		{
			return next->free(p);
		}
		else
		{
			return false;
		}
		
	}
	//Добавить изменение размеров
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
			structPtr->size = size + sizeof(struct dataBlock);
		}
	}

	dataBlock* findFirstFit(size_t size)
	{
		dataBlock* iterator = head;
		while (iterator != nullptr)
		{
			if (iterator->inUse==false && iterator->size >= size+sizeof(struct dataBlock))
				return iterator;
			iterator = iterator->next;
		}
		return nullptr;

	}

};


class Coalesce
{
public:
	Coalesce()
	{

	}

	~Coalesce()
	{
		while (startPage != nullptr)
		{
			MemoryPageCoalesce* next = startPage->next;
			VirtualFree(startPage, 0, MEM_RELEASE);
			delete(next);
			startPage = next;
		}
	}

	void init()
	{
		startPage = new MemoryPageCoalesce(CoalescePageSize);
	}

	void* alloc(size_t size)
	{
		size = allign(size);
		return startPage->alloc(size);
	}

	bool free(void* ptr)
	{
		return startPage->free(ptr);
	}


private:
	MemoryPageCoalesce* startPage;

	size_t allign(size_t size)
	{
		if (size % 8 == 0)
			return size;
		else
			return 8 * (size / 8 + 1);
	}
};