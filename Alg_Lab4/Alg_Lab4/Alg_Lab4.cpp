


#include <iostream>
#include "Memory_Allocator.h"


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
    fsa512.dumpBlocks();
    fsa512.free(c);
    fsa512.free(b);
    fsa512.free(e);

    
    fsa512.free(a);
    fsa512.free(d);
    fsa512.destroy();
}

void test_FSA_5elem_MemoryCorruption()
{
    std::cout << "Hello World!\n";
    FSA fsa512;
    fsa512.init(64);

    int* pa = (int*)fsa512.alloc();
    for (int i = 0; i < 18; i++)
        pa[i] = i;

    fsa512.free(pa);
    fsa512.destroy();
}

void test_FSA16_5elem_OK()
{
    std::cout << "Hello World!\n";
    FSA fsa512;
    fsa512.init(16);

    void* a = fsa512.alloc();
    void* b = fsa512.alloc();
    void* c = fsa512.alloc();
    void* d = fsa512.alloc();
    void* e = fsa512.alloc();
    
    fsa512.free(c);
    fsa512.dumpBlocks();
    fsa512.free(b);
    fsa512.free(e);


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
    FSA fsa16;
    fsa16.init(16);

    for (int i = 0; i <600; i++)
    {
        ptrarray[i] = fsa16.alloc();
    }

    for (int i = 0; i < 600; i += 2)
    {
        fsa16.free(ptrarray[i]);
    }
    fsa16.dumpBlocks();
    for (int i = 1; i < 600; i += 2)
    {
        fsa16.free(ptrarray[i]);
    }

    fsa16.destroy();
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
    allocator.free(b);
    void* g = allocator.alloc(1024);
    allocator.dumpBlocks();
    allocator.free(l);
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
    l = allocator.alloc(64);
    void* t = allocator.alloc(48);
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
        arr[i] = allocator.alloc((i + 1) * 1000);
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

void test_Coalesce_corruption()
{
    Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    allocator.free(l);
    int* k =(int*) allocator.alloc(120);
    
    for (int i = 0; i < 31; i++)
        k[i] = i;
    allocator.dumpBlocks();
    allocator.free(l);
    allocator.free(k);
    allocator.free(b);
    allocator.destroy();
}

void test_osMemoryAlloc()
{
    OSAllocator osAllocator;

    osAllocator.init();
    void* p = osAllocator.alloc(8);
    void* l = osAllocator.alloc(1000);
    void* q = osAllocator.alloc(10000);
    osAllocator.dumpBlocks();
    osAllocator.free(q);
    osAllocator.free(p);
    osAllocator.free(l);
    //osAllocator.free((void*)(1231 + 10));

}

void test_osMemoryAllocCorruption()
{
    OSAllocator osAllocator;

    osAllocator.init();
    void* p = osAllocator.alloc(8);
    *(int*)((char*)p - 8) = 154;
    void* l = osAllocator.alloc(1000);
    void* q = osAllocator.alloc(10000);
    osAllocator.dumpBlocks();
    osAllocator.free(q);
    osAllocator.free(p);
    osAllocator.free(l);
}

void test_MemoryAlloc_OK()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pi = (int*)allocator.alloc(sizeof(int));
    double* pd = (double*)allocator.alloc(sizeof(double));
    int* pa = (int*)allocator.alloc(10 * sizeof(int));

    allocator.dumpStat();
    allocator.dumpBlocks();

    allocator.free(pi);
    allocator.free(pd);
    allocator.free(pa);

    allocator.destroy();

}

void test_MemoryAlloc_NotInit()
{
    MemoryAllocator allocator;
    int* pi = (int*)allocator.alloc(sizeof(int));
    double* pd = (double*)allocator.alloc(sizeof(double));
    int* pa = (int*)allocator.alloc(10 * sizeof(int));

    allocator.dumpStat();
    allocator.dumpBlocks();

    allocator.free(pi);
    allocator.free(pd);
    allocator.free(pa);

    allocator.destroy();

}

void test_MemoryAlloc_Leak()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pi = (int*)allocator.alloc(sizeof(int));
    double* pd = (double*)allocator.alloc(sizeof(double));
    int* pa = (int*)allocator.alloc(10 * sizeof(int));

    allocator.dumpStat();
    allocator.dumpBlocks();

    allocator.free(pi);
    allocator.free(pa);

    allocator.destroy();

}

void test_MemoryWrongAddress()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pi = (int*)allocator.alloc(sizeof(int));
    double* pd = (double*)allocator.alloc(sizeof(double));
    int* pa = (int*)allocator.alloc(10 * sizeof(int));

    allocator.dumpStat();
    allocator.dumpBlocks();

    allocator.free(pi);
    allocator.free(pd);
    allocator.free((void*)((char*)pi+4141));

    allocator.destroy();

}

void test_MemoryCorruption()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pi = (int*)allocator.alloc(sizeof(int));
    double* pd = (double*)allocator.alloc(sizeof(double));
    int* pa = (int*)allocator.alloc(10 * sizeof(int));

    for (int i = 0; i < 17; i++)
        pa[i] = i;

    allocator.dumpStat();
    allocator.dumpBlocks();

    allocator.free(pi);
    allocator.free(pd);
    allocator.free(pa);

    allocator.destroy();

}

int main()
{
    test_Coalesce_severalPages();
    
}

