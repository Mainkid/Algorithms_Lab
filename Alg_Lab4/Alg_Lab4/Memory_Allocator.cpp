#pragma once
#include "Memory_Allocator.h"




MemoryAllocator::MemoryAllocator()
	{
		isInitialized = false;
	}

	MemoryAllocator::~MemoryAllocator()
	{
#ifdef DEBUG
		assert(!isInitialized && "Allocator should be deinitialized before destructor call!");
#endif
	}

	void MemoryAllocator::init()
	{
		fsa16.init(16);
		fsa32.init(32);
		fsa64.init(64);
		fsa128.init(128);
		fsa256.init(256);
		fsa512.init(512);
		coalesceAllocator.init();
		osAllocator.init();

		isInitialized = true;
	}

	void MemoryAllocator::destroy()
	{
#ifdef DEBUG
		assert(isInitialized && "Allocator should be initialized before destroy!");
#endif
		fsa16.destroy();
		fsa32.destroy();
		fsa64.destroy();
		fsa128.destroy();
		fsa256.destroy();
		fsa512.destroy();
		coalesceAllocator.destroy();
		osAllocator.destroy();

		isInitialized = false;
	}

	void* MemoryAllocator::alloc(size_t size)
	{
#ifdef DEBUG
		assert(isInitialized && "Allocator should be initialized before alloc!");
#endif

		if (size <= 16)
			return fsa16.alloc();
		else if (size <= 32)
			return fsa32.alloc();
		else if (size <= 64)
			return fsa64.alloc();
		else if (size <= 128)
			return fsa128.alloc();
		else if (size <= 256)
			return fsa256.alloc();
		else if (size <= 512)
			return fsa512.alloc();
		else if (size <= _10MB)
			return coalesceAllocator.alloc(size);
		else
			return osAllocator.alloc(size);

	}

	void MemoryAllocator::free(void* p)
	{
#ifdef DEBUG
		assert(isInitialized && "Allocator should be initialized before free!");
#endif

		if (fsa16.free(p))
			return;
		else if (fsa32.free(p))
			return;
		else if (fsa64.free(p))
			return;
		else if (fsa128.free(p))
			return;
		else if (fsa256.free(p))
			return;
		else if (fsa512.free(p))
			return;
		else if (coalesceAllocator.free(p))
			return;
		else if (osAllocator.free(p))
			return;
		else
		{
			assert(false && "Incorrect pointer!");
		}
	}

#ifdef DEBUG
	void MemoryAllocator::dumpStat() const
	{

		assert(isInitialized && "Allocator should be initialized before dumpStat!");

		fsa16.dumpStat();
		fsa32.dumpStat();
		fsa64.dumpStat();
		fsa128.dumpStat();
		fsa256.dumpStat();
		fsa512.dumpStat();
		coalesceAllocator.dumpStat();
		osAllocator.dumpStat();


		//Дописать


	}

	void MemoryAllocator::dumpBlocks() const
	{

		assert(isInitialized && "Allocator should be initialized before dumpBlocks!");


		fsa16.dumpBlocks();
		fsa32.dumpBlocks();
		fsa64.dumpBlocks();
		fsa128.dumpBlocks();
		fsa256.dumpBlocks();
		fsa512.dumpBlocks();
		coalesceAllocator.dumpBlocks();
		osAllocator.dumpBlocks();
	}
#endif

