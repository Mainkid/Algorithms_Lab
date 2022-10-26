#pragma once

struct MemoryOS
{
	MemoryOS* next;
	void* ptr;

};


class OSAllocator
{
public:
	OSAllocator()
	{
		head = nullptr;
	}

	~OSAllocator()
	{

	}

	void alloc(void* p)
	{
		if (head == nullptr)
		{
			head = MemoryOS
		}
	}
private:
	MemoryOS* head;
	MemoryOS* foot;
};