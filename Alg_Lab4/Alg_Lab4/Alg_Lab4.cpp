


#include <iostream>
#include "FSA_Allocator.h"
#include "Coalesce_Allocator.h"
#include "OSAllocator.h"

//remove and initialized;

void test_FSA_5elem_OK()
{
    std::cout << "Hello World!\n";
    FSA fsa512;
    fsa512.init(512);
    
    void* a = fsa512.alloc();
    void* b = fsa512.alloc();
    void* c = fsa512.alloc();
    void* d = fsa512.alloc();
    void* e = fsa512.alloc();

    fsa512.free(c);
    fsa512.free(b);
    fsa512.free(e);
     
    fsa512.free((void*)141341);
    std::cout << "OK" << std::endl;
    fsa512.dumpBlocks();
    fsa512.free(a);
    fsa512.free(d);
    fsa512.destroy();
}

void test_FSA_5elem_Leak()
{
    std::cout << "Hello World!\n";
    FSA fsa512;
    fsa512.init(512);

    void* a = fsa512.alloc();
    void* b = fsa512.alloc();
    void* c = fsa512.alloc();
    void* d = fsa512.alloc();
    void* e = fsa512.alloc();

    fsa512.free(c);
    fsa512.free(b);
    fsa512.free(e);

    fsa512.free((void*)141341);
    std::cout << "OK" << std::endl;
    fsa512.dumpBlocks();
    fsa512.destroy();
}

void test_FSA_5elem_NotDeinitialized()
{
    std::cout << "Hello World!\n";
    FSA fsa512;
    fsa512.init(512);

    void* a = fsa512.alloc();
    void* b = fsa512.alloc();
    void* c = fsa512.alloc();
    void* d = fsa512.alloc();
    void* e = fsa512.alloc();

    fsa512.free(c);
    fsa512.free(b);
    fsa512.free(e);

    fsa512.free((void*)141341);
    std::cout << "OK" << std::endl;
    fsa512.dumpBlocks();
    
}

void test_FSA_5elem_AllocNotInitialized()
{
    std::cout << "Hello World!\n";
    FSA fsa512;


    void* a = fsa512.alloc();
    void* b = fsa512.alloc();
    void* c = fsa512.alloc();
    void* d = fsa512.alloc();
    void* e = fsa512.alloc();

    fsa512.free(c);
    fsa512.free(b);
    fsa512.free(e);

    fsa512.free((void*)141341);
    std::cout << "OK" << std::endl;
    fsa512.dumpBlocks();
}

void test_FSA_Create3PagesAndDeleteOdd()
{
    void** ptrarray = new void* [600];
    FSA fsa512;
    fsa512.init(512);

    for (int i = 0; i <600; i++)
    {
        ptrarray[i] = fsa512.alloc();
    }

    for (int i = 0; i < 600; i += 2)
    {
        fsa512.free(ptrarray[i]);
    }
    fsa512.dumpBlocks();
    for (int i = 1; i < 600; i += 2)
    {
        fsa512.free(ptrarray[i]);
    }

    fsa512.destroy();
}

void test_FSA_FreeIncorrectIndex()
{

    FSA fsa512;
    fsa512.init(512);

    void* a = fsa512.alloc();
    void* b = fsa512.alloc();
    void* c = fsa512.alloc();
    void* d = fsa512.alloc();
    void* e = fsa512.alloc();

    fsa512.free((void*)141341);

    fsa512.dumpBlocks();
    fsa512.free(a);
    fsa512.free(b);
    fsa512.free(c);
    fsa512.free(e);
    fsa512.free(d);
    fsa512.destroy();

}

void test_Coalesce_3elem_OK()
{
    Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.dumpBlocks();
    allocator.free(l);
    allocator.free(b);
    allocator.free(g);
    allocator.destroy();
}

void test3()
{
    Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.free(l);
    allocator.free(b);

}

void test_Coalesce_6elem_unit_OK()
{
    Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.free(l);
    l = allocator.alloc(100);
    void* t = allocator.alloc(50);
    void* k = allocator.alloc(2024);
    allocator.dumpBlocks();
    allocator.free(l);
    allocator.free(b);
    allocator.free(g);
    allocator.free(t);
    allocator.free(k);
    allocator.destroy();

}

void test_Coalesce_3elem_leak()
{
    Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.dumpBlocks();
    allocator.free(l);
    allocator.free(b);
    allocator.destroy();
}

void test_Coalesce_NotDeinitialized()
{
    Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.dumpBlocks();
    allocator.free(l);
    allocator.free(b);
    allocator.free(g);
}

void test_Coalesce_AllocNotInitialized()
{
    Coalesce allocator;
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.dumpBlocks();
    allocator.free(l);
    allocator.free(b);
    allocator.free(g);
}

void test_Coalesce_severalPages()
{
    Coalesce allocator;
    allocator.init();
    void** arr = new void * [7];
    for (int i = 0; i < 7; i++)
    {
        arr[i] = allocator.alloc((i + 1) * 500);
    }
    allocator.dumpBlocks();
    for (int i = 0; i <7; i++)
    {
        allocator.free(arr[i]);
    }

    for (int i = 0; i < 7; i++)
    {
        arr[i] = allocator.alloc((i + 1) * 1000);
    }
    allocator.dumpBlocks();
    for (int i = 6; i >= 0; i--)
    {
        allocator.free(arr[i]);
    }

    allocator.destroy();
}

void test_MemoryAlloc()
{
    OSAllocator osAllocator;

    osAllocator.init();
    void* p = osAllocator.alloc(5500);
    void* l = osAllocator.alloc(1000);
    void* q = osAllocator.alloc(10000);
    osAllocator.dumpBlocks();
    osAllocator.free(q);
    osAllocator.free(l);
    osAllocator.free(p);
    

}

int main()
{
    //test3();
    test_MemoryAlloc();
    //test_MAXBLOCKS();
}

