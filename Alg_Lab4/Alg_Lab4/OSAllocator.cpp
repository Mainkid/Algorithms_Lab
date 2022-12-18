#pragma once
#include "OSAllocator.h"





	OSAllocator::OSAllocator()
	{
	}

	void OSAllocator::init()
	{

	}

	OSAllocator::~OSAllocator()
	{

	}



	void* OSAllocator::alloc(int _size)
	{
		return alloc(allign(_size), startPage);
	}

#ifdef DEBUG
	void* OSAllocator::alloc(int _size, osallocator::Header* page)
	{
		if (page == nullptr)
		{
			startPage = createPage(_size + sizeof(long long int) * 2);
			startPage->size -= sizeof(long long int) * 2;
			*((long long int*)((char*)startPage + sizeof(osallocator::Header))) = kMarker;
			*((long long int*)((char*)startPage + sizeof(osallocator::Header) + _size + sizeof(long long int))) = kMarker;
			return ((void*)((char*)startPage + sizeof(osallocator::Header) + sizeof(long long int)));
		}
		else if (page->next != nullptr)
		{
			return alloc(_size, page->next);
		}
		else
		{
			osallocator::Header* newPage = createPage(_size + sizeof(long long int) * 2);
			page->next = newPage;
			newPage->size -= sizeof(long long int) * 2;
			*((long long int*)((char*)newPage + sizeof(osallocator::Header))) = kMarker;
			*((long long int*)((char*)newPage + sizeof(osallocator::Header) + _size + sizeof(long long int))) = kMarker;
			return ((void*)((char*)newPage + sizeof(osallocator::Header) + sizeof(long long int)));
		}
	}

	void OSAllocator::dumpBlocks() const
	{
		std::cout << "OS Allocator: " << std::endl;

		osallocator::Header* curPage = startPage;
		while (curPage != nullptr)
		{
			std::cout << "Address: " << (void*)((char*)curPage + sizeof(osallocator::Header)) << ", size: " << curPage->size << std::endl;
			curPage = curPage->next;
		}

	}

	void OSAllocator::dumpStat() const
	{
		osallocator::Header* curPage = startPage;
		int pageCtr = 0;
		int totalSize = 0;
		std::cout << "OS Allocator: " << std::endl;

		while (curPage != nullptr)
		{
			pageCtr++;
			totalSize += curPage->size;
		}

		std::cout << "Pages: " << pageCtr << ", Total size: " << totalSize << std::endl << std::endl;

	}

	void OSAllocator::assertLeaks()
	{
		assert(startPage == nullptr && "MEMORY LEAK FOUND!");
	}

	void OSAllocator::checkMemoryCorruption(osallocator::Header* page)
	{
		assert(*((long long int*)((char*)page + sizeof(osallocator::Header))) == kMarker && "Memory Corruption!");
		assert(*((long long int*)((char*)page + sizeof(osallocator::Header) + page->size + sizeof(long long int))) == kMarker && "Memory Corruption!");
	}
#else
	void* OSAllocator::alloc(int _size, osallocator::Header* page)
	{
		if (page == nullptr)
		{
			startPage = createPage(_size);
			return ((void*)((char*)startPage + sizeof(struct osallocator::Header)));
		}
		else if (page->next != nullptr)
		{
			return alloc(_size, page->next);
		}
		else
		{
			osallocator::Header* newPage = createPage(_size);
			page->next = newPage;
			*((long long int*)(char*)newPage + sizeof(osallocator::Header)) = kMarker;
			return ((void*)((char*)newPage + sizeof(struct osallocator::Header)));
		}
	}
#endif


	osallocator::Header* OSAllocator::createPage(size_t _size)
	{
		int k = _size + sizeof(osallocator::Header);
		void* p = VirtualAlloc(nullptr, _size + sizeof(osallocator::Header), MEM_COMMIT, PAGE_READWRITE);
		osallocator::Header* page = (osallocator::Header*)(p);
		page->next = nullptr;
		page->size = _size;
		return page;
	}

	bool OSAllocator::free(void* p)
	{
#ifdef DEBUG
		int padding = sizeof(long long int);
#else
		int padding = 0;
#endif

		osallocator::Header* curPage = startPage;
		osallocator::Header* oldPage = startPage;
		while (curPage != nullptr)
		{
			if ((char*)curPage + sizeof(osallocator::Header) + padding == p)
			{


				if (curPage == oldPage)
					startPage = curPage->next;
				else
					oldPage->next = curPage->next;

#ifdef DEBUG
				checkMemoryCorruption(curPage);
#endif
				VirtualFree(curPage, 0, MEM_RELEASE);
				return true;
			}
			else
			{
				oldPage = curPage;
				curPage = curPage->next;
			}
		}

		return false;

	}

	void OSAllocator::destroy()
	{
#ifdef DEBUG
		assertLeaks();
#endif

		osallocator::Header* page = startPage;
		while (page != nullptr)
		{
			osallocator::Header* oldPage = page;
			page = page->next;
			VirtualFree(oldPage, 0, MEM_RELEASE);

		}
	}


	size_t OSAllocator::allign(size_t size)
	{
		if (size % 8 == 0)
			return size;
		else
			return 8 * (size / 8 + 1);
	}
