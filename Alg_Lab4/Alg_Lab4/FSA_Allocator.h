#pragma once
#include <iostream>
#include "windows.h"
#include <cassert> 
#include "Constants.h"


struct Header
{
	Header* next; // 8 байт
	int fHead;    // 4 байта
	int numInitialized; // 4 байта
};



class FSA
{
public:

	FSA();

	~FSA();

	void init(size_t _blockSize);

#ifdef DEBUG
	

	Header* createPage(size_t _blockSize);
	

	void* alloc(Header* page);
	

	bool free(void* p, Header* page);
	

	void dumpBlocks() const;
	
	void dumpStat() const;
	

	void assertLeaks();

	void checkMemoryCorruption();
	

#else

	Header* createPage(size_t _blockSize);

	void* alloc(Header* page);

	bool free(void* p, Header* page);
	

#endif

	

	void destroy();
	

	void* alloc();

	bool free(void* p);




private:
	Header* startPage;  // <--- Указатель на хедер страницы
	size_t blockSize;   // <--- Размер блока
};