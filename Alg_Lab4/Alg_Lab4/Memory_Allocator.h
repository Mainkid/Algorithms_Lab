#pragma once
#include "Coalesce_Allocator.h"
#include "FSA_Allocator.h"
#include "OSAllocator.h"



class MemoryAllocator {

public:
	MemoryAllocator();

	virtual ~MemoryAllocator();

	virtual void init();

	virtual void destroy();

	virtual void* alloc(size_t size);

	virtual void free(void* p);

#ifdef DEBUG
	virtual void dumpStat() const;

	virtual void dumpBlocks() const;

#endif

private:
	FSA fsa16;
	FSA fsa32;
	FSA fsa64;
	FSA fsa128;
	FSA fsa256;
	FSA fsa512;

	Coalesce coalesceAllocator;

	OSAllocator osAllocator;

	bool isInitialized;

};