#pragma once
#include <cassert>
#include <iostream>
#include <Windows.h>
#include "Constants.h"

//Поменять кб в байтах

struct dataBlock
{
	dataBlock* next;   // next + prev for coalesce
	dataBlock* prev;   //                                 40 bytes
	dataBlock* nextFL; // nextFreeList;
	dataBlock* prevFL; // prevFreeList;
	int size;
	bool inUse;
	
};

namespace coalesceNS{

	struct Header
	{
		Header* next;
		dataBlock* head;   //                                    24 bytes
		dataBlock* headFL; //Free list head
	};
}



class Coalesce
{
public:
	Coalesce();

	~Coalesce();

	void init();

#ifdef DEBUG
	coalesceNS::Header* createPage(size_t _size);

	void* alloc(size_t _size, coalesceNS::Header* page);

	dataBlock* findFirstFit(size_t size);

	bool free(void* p, coalesceNS::Header* page);
	
	void tryCoalesce(dataBlock* structPtr, coalesceNS::Header* page);

	void dumpStat() const;

	void dumpBlocks() const;

	void assertLeaks(coalesceNS::Header* page);

	void checkMemoryCorruption();

#else
	coalesceNS::Header* createPage(size_t _size);

	void* alloc(size_t _size, coalesceNS::Header* page);

	dataBlock* findFirstFit(size_t size);

	bool free(void* p, coalesceNS::Header* page);

	void tryCoalesce(dataBlock* structPtr, coalesceNS::Header* page);

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
	

	void* alloc(size_t _size);

	



	bool free(void* p);

	

	bool isCorrectPtr(void* p, coalesceNS::Header* page);

	void destroy();



private:
	coalesceNS::Header* startPage;
	size_t allign(size_t size);
};