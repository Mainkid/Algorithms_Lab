#pragma once
#include "Coalesce_Allocator.h"
#include "FSA_Allocator.h"
#include "OSAllocator.h"

#define _10MB 1048576

class MemoryAllocator {

public:
	MemoryAllocator()
	{

	}

	virtual ~MemoryAllocator()
	{

	}

	virtual void init()
	{
		fsa16.init(16);
		fsa32.init(32);
		fsa64.init(64);
		fsa128.init(128);
		fsa256.init(256);
		fsa512.init(512);
		coalesceAllocator.init();
		osAllocator.init();
	}

	virtual void destroy()
	{

	}

	virtual void* alloc(size_t size)
	{
		if (size <= 16)
			fsa16.alloc();
		else if (size <= 32)
			fsa32.alloc();
		else if (size <= 64)
			fsa64.alloc();
		else if (size <= 128)
			fsa128.alloc();
		else if (size <= 256)
			fsa256.alloc();
		else if (size <= 512)
			fsa512.alloc();
		else if (size <= _10MB)
			coalesceAllocator.alloc(size);
		else
			osAllocator.alloc(size);

	}

	virtual void free(void* p)
	{
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
			//assert!
		}
	}

	virtual void dumpStat() const
	{
		fsa16.dumpBlocks();
		fsa32.dumpBlocks();
		fsa64.dumpBlocks();
		fsa128.dumpBlocks();
		fsa256.dumpBlocks();
		fsa512.dumpBlocks();
		coalesceAllocator.dumpBlocks();

	}

	virtual void dumpBlocks() const;

private:
	FSA fsa16;
	FSA fsa32;
	FSA fsa64;
	FSA fsa128;
	FSA fsa256;
	FSA fsa512;

	Coalesce coalesceAllocator;

	OSAllocator osAllocator;

};