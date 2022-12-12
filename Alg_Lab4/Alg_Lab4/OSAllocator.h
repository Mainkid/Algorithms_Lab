#pragma once
#include "windows.h"
#include <iostream>

struct MemoryOS
{
	MemoryOS* next=nullptr;
	void* ptr=nullptr;
	int size;

	MemoryOS() {}

	void* alloc(int _size)
	{
		if (!ptr)
		{
			ptr = VirtualAlloc(nullptr, _size, MEM_COMMIT, PAGE_READWRITE);
			size = _size;
			return ptr;
		}
		else if (next==nullptr)
		{
			next = new MemoryOS();
			
		}
		return next->alloc(_size);
	}
	
	

};


class OSAllocator
{
public:
	OSAllocator()
	{
		
	}

	void init()
	{
		head = new MemoryOS();
	}

	~OSAllocator()
	{

	}

	void* alloc(int _size)
	{
		return head->alloc(_size);
	}

	bool free(void* p)
	{
		MemoryOS* curPage = head;
		MemoryOS* oldPage = head;
		while (curPage != nullptr)
		{
			if (curPage->ptr == p)
			{
				VirtualFree(curPage->ptr, 0, MEM_RELEASE);
				if (curPage == oldPage)
					head = curPage->next;
				else
					oldPage->next = curPage->next;
				delete curPage;
				
			}
			oldPage = curPage;
			curPage = curPage->next;
		}

		return true;

	}

	void destroy()
	{
		MemoryOS* curPage = head;
		while (curPage != nullptr)
		{
			MemoryOS* oldPage = curPage;
			VirtualFree(curPage->ptr, 0, MEM_RELEASE);
			curPage = curPage->next;
			delete(oldPage);
		}
	}

	void dumpBlocks() const
	{
		std::cout << "OS Allocation: " << std::endl;

		MemoryOS* curPage = head;
		while (curPage != nullptr)
		{
			std::cout << "Address: " << curPage->ptr << ", size: " << curPage->size << std::endl;
			curPage=curPage->next;
		}
	}

private:
	MemoryOS* head;
	
};