#pragma once
#include "FSA_Allocator.h"





	FSA::FSA() {
	}

	FSA::~FSA()
	{

	}

	void FSA::init(size_t _blockSize)
	{
		this->blockSize = _blockSize;

		//void* p = VirtualAlloc(nullptr,sizeof(int*)*2*BLOCKS+ _blockSize * BLOCKS + sizeof(Header), MEM_COMMIT, PAGE_READWRITE);

		startPage = createPage(_blockSize);

	}

#ifdef DEBUG


	Header* FSA::createPage(size_t _blockSize)
	{
		void* p = VirtualAlloc(nullptr, (sizeof(long long int) * 2 + _blockSize) * BLOCKS + sizeof(Header), MEM_COMMIT, PAGE_READWRITE);
		Header* page = static_cast<Header*>(p);
		page->fHead = 0;
		page->next = nullptr;
		page->numInitialized = 0;
		for (int i = 0; i < BLOCKS; i++)
		{
			*(reinterpret_cast<long long int*>((char*)page + sizeof(Header) + i * (_blockSize + sizeof(long long int) * 2))) = kMarker;
			*(reinterpret_cast<long long int*>((char*)page + sizeof(Header) + i * (_blockSize + sizeof(long long int) * 2) + sizeof(long long int))) = i + 1;
			*(reinterpret_cast<long long int*>((char*)page + sizeof(Header) + i * (_blockSize + sizeof(long long int) * 2) + sizeof(long long int) * 2)) = kBusyBlockMarker;
			*(reinterpret_cast<long long int*>((char*)page + sizeof(Header) + i * (_blockSize + sizeof(long long int) * 2) + sizeof(long long int) + _blockSize)) = kMarker;
		}

		return page;
	}

	void* FSA::alloc(Header* page)
	{
		void* block = nullptr;


		if (page->numInitialized == BLOCKS)
		{
			if (page->next == nullptr)
			{
				page->next = createPage(blockSize);
			}
			return alloc(page->next);
		}
		else
		{
			page->numInitialized += 1;
			long long int tmp;
			//int prevHead = page->fHead;

			if (page->numInitialized < BLOCKS)
			{
				tmp = *(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + page->fHead * (blockSize + sizeof(long long int) * 2) + sizeof(long long int)));
				block = reinterpret_cast<void*>((char*)page + sizeof(struct Header) + page->fHead * (blockSize + sizeof(long long int) * 2) + sizeof(long long int));
				*(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + page->fHead * (blockSize + sizeof(long long int) * 2) + sizeof(long long int) * 2)) = -1;
				page->fHead = tmp;
			}
			else
			{
				block = reinterpret_cast<void*>((char*)page + sizeof(struct Header) + page->fHead * (blockSize + sizeof(long long int) * 2) + sizeof(long long int));
				page->fHead = -1;

			}

			return block;
		}
	}

	bool FSA::free(void* p, Header* page)
	{
		if ((char*)p - (char*)page < BLOCKS * (blockSize + sizeof(long long int) * 2) + sizeof(struct Header))
		{
			if (((char*)p - (char*)page - sizeof(struct Header) - sizeof(long long int)) % (blockSize + sizeof(long long int) * 2) != 0)
			{
				//std::cout << p << " Incorrect address!" << std::endl;
				return false;

			}

			else
			{

				page->numInitialized -= 1;

				long long int index = (((char*)p - (char*)page - sizeof(struct Header)) / (blockSize + sizeof(long long int) * 2));


				long long int tmp = page->fHead;
				page->fHead = index;
				*(reinterpret_cast<long long*>((char*)page + sizeof(struct Header) + index * (blockSize + sizeof(long long int) * 2) + sizeof(long long int))) = tmp;
				*(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + index * (blockSize + sizeof(long long int) * 2) + sizeof(long long int) * 2)) = kBusyBlockMarker;

				return true;
			}
		}
		else if (page->next != nullptr)
		{
			return free(p, page->next);
		}
		else
		{
			//std::cout << p << " Incorrect address!" << std::endl;
			return false;
		}
	}

	void FSA::dumpBlocks() const
	{




		std::cout << "FSA" << blockSize << " allocator:" << std::endl;

		Header* curPage = startPage;
		int pageCounter = 1;

		while (curPage != nullptr)
		{
			for (int i = 0; i < BLOCKS; i++)
			{

				if (*(reinterpret_cast<long long int*>((char*)curPage + sizeof(struct Header) + i * (blockSize + sizeof(long long int) * 2) + sizeof(long long int) * 2)) != kBusyBlockMarker)
				{
					std::cout << "Page: " << pageCounter << ", " << "block address: " << (void*)((char*)curPage + sizeof(struct Header) + i * (blockSize + sizeof(long long int) * 2)) << std::endl;
				}
			}
			pageCounter += 1;
			curPage = curPage->next;
		}

	}
	void FSA::dumpStat() const
	{
		Header* curPage = startPage;
		int init = 0;
		int free = 0;
		int pageCtr = 0;
		while (curPage)
		{
			init += curPage->numInitialized;
			free += BLOCKS - curPage->numInitialized;

			curPage = curPage->next;
			pageCtr++;
		}

		std::cout << "FSA" << blockSize << " allocator: " << std::endl;
		std::cout << "Pages: " << pageCtr << ", Initialized: " << init << ", Free: " << free << std::endl;
		std::cout << std::endl;
	}

	void FSA::assertLeaks()
	{
		Header* curPage = startPage;

		while (curPage != nullptr)
		{
			for (int i = 0; i < BLOCKS; i++)
			{
				long long int marker = *(reinterpret_cast<long long int*>((char*)curPage + sizeof(struct Header) + i * (blockSize + sizeof(long long int) * 2) + sizeof(long long int) * 2));
				assert(marker == kBusyBlockMarker && "FSA MEMORY LEAK FOUND");
			}

			curPage = curPage->next;
		}
	}

	void FSA::checkMemoryCorruption()
	{
		Header* curPage = startPage;
		while (curPage != nullptr)
		{

			for (int i = 0; i < BLOCKS; i++)
			{

				long long int firstMarker = *(reinterpret_cast<long long int*>((char*)curPage + sizeof(Header) + i * (blockSize + sizeof(long long int) * 2)));
				long long int lastMarker = *(reinterpret_cast<long long int*>((char*)curPage + sizeof(Header) + i * (blockSize + sizeof(long long int) * 2) + sizeof(long long int) + blockSize));
				long long int body1 = *(reinterpret_cast<long long int*>((char*)curPage + sizeof(Header) + i * (blockSize + sizeof(long long int) * 2) + sizeof(long long int)));
				long long int body2 = *(reinterpret_cast<long long int*>((char*)curPage + sizeof(Header) + i * (blockSize + sizeof(long long int) * 2) + sizeof(long long int) * 2));
				assert(firstMarker == kMarker && "Memory corruption found!");
				assert(lastMarker == kMarker && "Memory corruption found!");

			}
			curPage = curPage->next;
		}
	}

#else

	Header* FSA::createPage(size_t _blockSize)
	{
		void* p = VirtualAlloc(nullptr, _blockSize * BLOCKS + sizeof(Header), MEM_COMMIT, PAGE_READWRITE);
		Header* page = static_cast<Header*>(p);
		page->fHead = 0;
		page->next = nullptr;
		page->numInitialized = 0;
		for (int i = 0; i < BLOCKS; i++)
		{
			*(reinterpret_cast<long long int*>((char*)page + sizeof(Header) + i * _blockSize)) = i + 1;
			*(reinterpret_cast<long long int*>((char*)page + sizeof(Header) + i * _blockSize + sizeof(long long int))) = kBusyBlockMarker;
		}

		return page;
	}

	void* FSA::alloc(Header* page)
	{
		void* block = nullptr;


		if (page->numInitialized == BLOCKS)
		{
			if (page->next == nullptr)
			{
				page->next = createPage(blockSize);
			}
			return alloc(page->next);
		}
		else
		{
			page->numInitialized += 1;
			long long int tmp;
			long long int prevHead = page->fHead;

			if (page->numInitialized < BLOCKS)
			{
				tmp = *(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + page->fHead * blockSize));
				block = reinterpret_cast<void*>((char*)page + sizeof(struct Header) + page->fHead * blockSize);
				*(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + page->fHead * blockSize + sizeof(long long int))) = -1;
				page->fHead = tmp;
			}
			else
			{
				block = reinterpret_cast<void*>((char*)page + sizeof(struct Header) + page->fHead * blockSize);
				page->fHead = -1;

			}

			return block;
		}
	}

	bool FSA::free(void* p, Header* page)
	{
		if ((char*)p - (char*)page < BLOCKS * blockSize + sizeof(struct Header))
		{
			if (((char*)p - (char*)page - sizeof(struct Header)) % blockSize != 0)
			{
				std::cout << p << " Incorrect address!" << std::endl;
				return false;

			}

			else
			{

				page->numInitialized -= 1;

				long long int index = (((char*)p - (char*)page - sizeof(struct Header)) / blockSize);


				long long int tmp = page->fHead;
				page->fHead = index;
				*(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + index * blockSize)) = tmp;
				*(reinterpret_cast<long long int*>((char*)page + sizeof(struct Header) + index * blockSize + sizeof(long long int))) = kBusyBlockMarker;

				return true;
			}
		}
		else if (page->next != nullptr)
		{
			return free(p, page->next);
		}
		else
		{
			std::cout << p << " Incorrect address!" << std::endl;
			return false;
		}
	}

#endif



	void FSA::destroy()
	{
#ifdef DEBUG
		checkMemoryCorruption();
		assertLeaks();
#endif
		while (startPage != nullptr)
		{
			Header* next = startPage->next;
			VirtualFree(startPage, 0, MEM_RELEASE);
			startPage = next;
		}

	}

	void* FSA::alloc()
	{
		return alloc(startPage);
	}

	bool FSA::free(void* p)
	{
		return free(p, startPage);
	}



