#pragma once
#include "windows.h"
#include <cassert> 
#include <iostream>
#include "Constants.h"

namespace osallocator {
	struct Header
	{
		Header* next;
		size_t size;

	};
}


class OSAllocator
{
public:
	OSAllocator();

	~OSAllocator();

	void init();

	void* alloc(int _size);

#ifdef DEBUG
	void* alloc(int _size, osallocator::Header* page);

	void dumpBlocks() const;

	void dumpStat() const;

	void assertLeaks();

	void checkMemoryCorruption(osallocator::Header* page);

#else
	void* alloc(int _size, osallocator::Header* page);
	
#endif


	osallocator::Header* createPage(size_t _size);

	bool free(void* p);

	void destroy();


private:
	osallocator::Header* startPage;


	size_t allign(size_t size);
};