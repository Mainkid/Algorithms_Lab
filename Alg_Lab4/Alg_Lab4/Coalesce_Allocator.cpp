#pragma once
#include "Coalesce_Allocator.h"



	Coalesce::Coalesce()
	{

	}

	Coalesce::~Coalesce()
	{

	}

	void Coalesce::init()
	{

		startPage = createPage(CoalescePageSize);

	}

#ifdef DEBUG
	coalesceNS::Header* Coalesce::createPage(size_t _size)
	{
		void* p = VirtualAlloc(nullptr, _size, MEM_COMMIT, PAGE_READWRITE);
		coalesceNS::Header* page = static_cast<coalesceNS::Header*>(p);
		page->next = nullptr;
		page->head = reinterpret_cast<dataBlock*>((char*)page + sizeof(coalesceNS::Header));

		page->head->inUse = false;
		page->head->next = nullptr;
		page->head->prev = nullptr;
		page->head->nextFL = nullptr;
		page->head->size = _size - sizeof(struct coalesceNS::Header) - sizeof(struct dataBlock) - sizeof(long long int) * 2;
		page->headFL = page->head;

		*(reinterpret_cast<long long int*>((char*)page + sizeof(coalesceNS::Header) + sizeof(dataBlock))) = kMarker;
		*(reinterpret_cast<long long int*>((char*)page + sizeof(coalesceNS::Header) + sizeof(dataBlock) + sizeof(long long int) + page->head->size)) = kMarker;
		return page;
	} //can unit

	void* Coalesce::alloc(size_t _size, coalesceNS::Header* page)
	{


		dataBlock* block = findFirstFit(_size);

		if (block == nullptr)
		{
			coalesceNS::Header* iter = page;
			while (iter->next != nullptr)
				iter = iter->next;
			iter->next = createPage(CoalescePageSize);
			return alloc(_size, iter->next);
		}

		else
		{
			size_t oldSize = block->size;
			block->inUse = true;
			block->size = _size;
			dataBlock* oldNext = block->next;
			if (oldNext != ((dataBlock*)((char*)block + block->size)))
			{
				dataBlock* newDB = (dataBlock*)((char*)block + block->size + sizeof(struct dataBlock) + sizeof(long long int) * 2);

				block->next = newDB;
				newDB->prev = block;
				newDB->next = oldNext;
				newDB->inUse = false;
				newDB->size = oldSize - _size - sizeof(struct dataBlock) - sizeof(long long int) * 2; //????
				if (oldNext)
					oldNext->prev = newDB;
				newDB->nextFL = block->nextFL;
				newDB->prevFL = block->prevFL;

				*(reinterpret_cast<long long int*>((char*)newDB + sizeof(dataBlock))) = kMarker;
				*(reinterpret_cast<long long int*>((char*)newDB - sizeof(long long int))) = kMarker;

				if (block->nextFL)
					block->nextFL->prevFL = newDB;
				if (block->prevFL)
					block->prevFL->nextFL = newDB;

				if (!page->headFL->inUse)
				{
					newDB->nextFL = page->headFL;
					page->headFL->prevFL = newDB;

				}
				page->headFL = newDB;
			}
			else
			{
				std::cout << "!";
			}

			return (void*)((char*)block + sizeof(struct dataBlock) + sizeof(long long int));

		}
	}
	//can unit
	dataBlock* Coalesce::findFirstFit(size_t size)
	{
		dataBlock* iterator;
		coalesceNS::Header* iteratorPage = startPage;
		while (iteratorPage != nullptr)
		{
			iterator = iteratorPage->headFL;
			while (iterator != nullptr)
			{
				if (iterator->size >= size + sizeof(struct dataBlock) + sizeof(long long int))
					return iterator;
				iterator = iterator->nextFL;
			}
			iteratorPage = iteratorPage->next;
		}
		return nullptr;

	} // can unit
	//can unit
	bool Coalesce::free(void* p, coalesceNS::Header* page)
	{
		if ((char*)p - (char*)page < CoalescePageSize - sizeof(coalesceNS::Header))
		{
			//#ifdef DEBUG
			//			assert(isCorrectPtr(p) && "Incorrect address!");
			//#endif
			dataBlock* freeBlock = ((struct dataBlock*)((char*)p - sizeof(struct dataBlock) - sizeof(long long int)));
			freeBlock->nextFL = page->headFL;
			page->headFL->prevFL = freeBlock;
			page->headFL = freeBlock;
			freeBlock->inUse = false;
			tryCoalesce(freeBlock, page);
		}
		else if (page->next != nullptr)
		{
			return free(p, page->next);
		}
		else
		{
			return false;
		}
		return true;
	}
	//can unit
	void Coalesce::tryCoalesce(dataBlock* structPtr, coalesceNS::Header* page)
	{
		while (structPtr->prev != nullptr && structPtr->prev->inUse == false)
		{
			size_t size = structPtr->size;
			if (structPtr->next != nullptr)
			{
				structPtr->next->prev = structPtr->prev;
			}

			structPtr->prev->next = structPtr->next;
			structPtr->prev->size += size + sizeof(struct dataBlock) + sizeof(long long int) * 2;

			if (structPtr->prevFL != nullptr)
				structPtr->prevFL->nextFL = structPtr->nextFL;
			if (structPtr->nextFL != nullptr)
				structPtr->nextFL->prevFL = structPtr->prevFL;

			if (structPtr == page->headFL)
				page->headFL = structPtr->prev;

			structPtr = structPtr->prev;


		}

		while (structPtr->next != nullptr && structPtr->next->inUse == false)
		{

			size_t size = structPtr->next->size;
			if (structPtr->next->next != nullptr)
			{
				structPtr->next->next->prev = structPtr;

			}


			if (structPtr->next && structPtr->next->prevFL)
				structPtr->next->prevFL->nextFL = structPtr->next->nextFL;
			if (structPtr->next && structPtr->next->nextFL)
				structPtr->next->nextFL->prevFL = structPtr->next->prevFL;


			structPtr->next = structPtr->next->next;
			structPtr->size += size + sizeof(struct dataBlock) + sizeof(long long int) * 2;
		}
	}

	void Coalesce::dumpStat() const
	{
		std::cout << "Coalesce allocator:" << std::endl;
		int pageCtr = 0;
		int totalMemSize = 0;


		coalesceNS::Header* curPage = startPage;

		while (curPage)
		{
			dataBlock* iter = curPage->head;
			pageCtr++;
			while (iter != nullptr)
			{
				if (iter->inUse)
					totalMemSize += iter->size;
				iter = iter->next;
			}
			curPage = curPage->next;
		}
		std::cout << "Pages: " << pageCtr << ", Total Size: " << totalMemSize << std::endl;


	}

	void Coalesce::dumpBlocks() const
	{

		std::cout << "Coalesce allocator:" << std::endl;
		int pageCounter = 1;
		coalesceNS::Header* iterPage = startPage;

		while (iterPage != nullptr)
		{
			dataBlock* iterHead = iterPage->head;
			while (iterHead != nullptr)
			{
				if (iterHead->inUse)
				{
					std::cout << "Page: " << pageCounter << ", " << "address: " << (void*)((char*)iterHead + sizeof(struct dataBlock) + sizeof(long long int)) << std::endl;
				}
				iterHead = iterHead->next;

			}
			pageCounter += 1;
			iterPage = iterPage->next;
		}
	}

	void Coalesce::assertLeaks(coalesceNS::Header* page)
	{
		dataBlock* iter = page->head;
		while (iter != nullptr)
		{
			if (iter->inUse)
				assert(!iter->inUse && "COALESCE MEMORY LEAK FOUND");
			iter = iter->next;

		}
		if (page->next != nullptr)
			assertLeaks(page->next);
	}

	void Coalesce::checkMemoryCorruption()
	{
		coalesceNS::Header* curPage = startPage;
		while (curPage != nullptr)
		{
			dataBlock* iter = curPage->head;

			while (iter != nullptr)
			{
				long long int marker1 = *(reinterpret_cast<long long int*>((char*)iter + sizeof(dataBlock)));
				long long int marker2 = *(reinterpret_cast<long long int*>((char*)iter + sizeof(dataBlock) + sizeof(long long int) + iter->size));
				assert(marker1 == kMarker && "MEMORY CORRUPTION");
				assert(marker2 == kMarker && "MEMORY CORRUPTION");

				iter = iter->next;
			}
			curPage = curPage->next;

		}
	}

#else
	coalesceNS::Header* Coalesce::createPage(size_t _size)
	{
		void* p = VirtualAlloc(nullptr, _size, MEM_COMMIT, PAGE_READWRITE);
		coalesceNS::Header* page = static_cast<coalesceNS::Header*>(p);
		page->next = nullptr;
		page->head = reinterpret_cast<dataBlock*>((char*)page + sizeof(struct coalesceNS::Header));
		page->head->inUse = false;
		page->head->next = nullptr;
		page->head->prev = nullptr;
		page->head->nextFL = nullptr;
		page->head->size = _size - sizeof(struct coalesceNS::Header) - sizeof(struct dataBlock);
		page->headFL = page->head;
		return page;
	}

	void* Coalesce::alloc(size_t _size, coalesceNS::Header* page)
	{


		dataBlock* block = findFirstFit(_size);

		if (block == nullptr)
		{
			coalesceNS::Header* iter = page;
			while (iter->next != nullptr)
				iter = iter->next;
			iter->next = createPage(CoalescePageSize);
			return alloc(_size, iter->next);
		}

		else
		{
			size_t oldSize = block->size;
			block->inUse = true;
			block->size = _size;
			dataBlock* oldNext = block->next;
			if (oldNext != ((dataBlock*)((char*)block + block->size)))
			{
				dataBlock* newDB = (dataBlock*)((char*)block + block->size + sizeof(struct dataBlock));
				block->next = newDB;
				newDB->prev = block;
				newDB->next = oldNext;
				newDB->inUse = false;
				newDB->size = oldSize - _size - sizeof(struct dataBlock);
				if (oldNext)
					oldNext->prev = newDB;
				newDB->nextFL = block->nextFL;
				newDB->prevFL = block->prevFL;

				if (block->nextFL)
					block->nextFL->prevFL = newDB;
				if (block->prevFL)
					block->prevFL->nextFL = newDB;

				if (!page->headFL->inUse)
				{
					newDB->nextFL = page->headFL;
					page->headFL->prevFL = newDB;

				}
				page->headFL = newDB;
			}
			else
			{
				std::cout << "!";
			}

			return (void*)((char*)block + sizeof(struct dataBlock));

		}
	}

	dataBlock* Coalesce::findFirstFit(size_t size)
	{
		dataBlock* iterator;
		coalesceNS::Header* iteratorPage = startPage;
		while (iteratorPage != nullptr)
		{
			iterator = iteratorPage->headFL;
			while (iterator != nullptr)
			{
				if (iterator->size >= size + sizeof(struct dataBlock))
					return iterator;
				iterator = iterator->nextFL;
			}
			iteratorPage = iteratorPage->next;
		}
		return nullptr;

	}

	bool Coalesce::free(void* p, coalesceNS::Header* page)
	{
		if ((char*)p - (char*)page < CoalescePageSize - sizeof(coalesceNS::Header))
		{
			//#ifdef DEBUG
			//			assert(isCorrectPtr(p) && "Incorrect address!");
			//#endif
			dataBlock* freeBlock = ((struct dataBlock*)((char*)p - sizeof(struct dataBlock)));
			freeBlock->nextFL = page->headFL;
			page->headFL->prevFL = freeBlock;
			page->headFL = freeBlock;
			freeBlock->inUse = false;
			tryCoalesce(freeBlock, page);
		}
		else if (page->next != nullptr)
		{
			return free(p, page->next);
		}
		else
		{
			//#ifdef DEBUG
			//			assert(isCorrectPtr(p) && "Incorrect address!");
			//#endif
			return false;
		}
		return true;
	}

	void Coalesce::tryCoalesce(dataBlock* structPtr, coalesceNS::Header* page)
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

			if (structPtr->prevFL != nullptr)
				structPtr->prevFL->nextFL = structPtr->nextFL;
			if (structPtr->nextFL != nullptr)
				structPtr->nextFL->prevFL = structPtr->prevFL;

			if (structPtr == page->headFL)
				page->headFL = structPtr->prev;

			structPtr = structPtr->prev;


		}

		while (structPtr->next != nullptr && structPtr->next->inUse == false)
		{

			size_t size = structPtr->next->size;
			if (structPtr->next->next != nullptr)
			{
				structPtr->next->next->prev = structPtr;

			}


			if (structPtr->next && structPtr->next->prevFL)
				structPtr->next->prevFL->nextFL = structPtr->next->nextFL;
			if (structPtr->next && structPtr->next->nextFL)
				structPtr->next->nextFL->prevFL = structPtr->next->prevFL;


			structPtr->next = structPtr->next->next;
			structPtr->size += size + sizeof(struct dataBlock);
		}
	}

	/*void dumpBlocks() const
	{

		std::cout << "Coalesce allocator:" << std::endl;
		int pageCounter = 1;
		coalesceNS::Header* iterPage = startPage;

		while (iterPage != nullptr)
		{
			dataBlock* iterHead = iterPage->head;
			while (iterHead != nullptr)
			{
				if (iterHead->inUse)
				{
					std::cout << "Page: " << pageCounter << ", " << "address: " << (void*)((char*)iterHead + sizeof(struct dataBlock)) << std::endl;
				}
				iterHead = iterHead->next;

			}
			pageCounter += 1;
			iterPage = iterPage->next;
		}
	}*/


#endif


	void* Coalesce::alloc(size_t _size)
	{
		_size = allign(_size);
		return alloc(_size, startPage);
	}





	bool Coalesce::free(void* p)
	{
#ifdef DEBUG
		checkMemoryCorruption();
#endif
		//		return startPage->free(ptr);

		return free(p, startPage);
	}



	bool Coalesce::isCorrectPtr(void* p, coalesceNS::Header* page)
	{
		dataBlock* iterHead = page->head;
		while (iterHead != nullptr)
		{
			if ((void*)((char*)iterHead + sizeof(struct dataBlock)) == p)
				return true;
			iterHead = iterHead->next;
		}
		return false;
	}

	void Coalesce::destroy()
	{
#ifdef DEBUG

		assertLeaks(startPage);
		checkMemoryCorruption();
#endif
		while (startPage != nullptr)
		{
			coalesceNS::Header* next = startPage->next;
			VirtualFree(startPage, 0, MEM_RELEASE);
			startPage = next;
		}

		return;
	}

#ifdef DEBUG

#endif


	coalesceNS::Header* startPage;

	size_t Coalesce::allign(size_t size)
	{
		if (size % 8 == 0)
			return size;
		else
			return 8 * (size / 8 + 1);
	}
