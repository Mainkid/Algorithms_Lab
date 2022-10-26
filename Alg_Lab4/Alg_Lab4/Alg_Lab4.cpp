


#include <iostream>
#include "FSA_Allocator.h"
#include "Coalesce_Allocator.h"

void test1()
{
    std::cout << "Hello World!\n";
    FSA fsa512;
    fsa512.init(512);
    void* l = fsa512.alloc();
    void* k = fsa512.alloc();
    void* g = fsa512.alloc();
    for (int i = 0; i < 6; i++)
        void* p = fsa512.alloc();
    //fsa512.free(l);
    fsa512.free(g);

    std::cout << "OK" << std::endl;
    fsa512.dumpBlocks();
}

int main()
{
    /*Coalesce allocator;
    allocator.init();
    void* l = allocator.alloc(240);
    void* b = allocator.alloc(512);
    void* g = allocator.alloc(1024);
    allocator.free(l);
    allocator.free(b);*/

    test1();
}

