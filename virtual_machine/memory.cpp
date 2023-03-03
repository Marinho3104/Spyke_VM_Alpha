#include "memory.h"

#include "program_definitions.h"

#include <sys/mman.h>
#include <iostream>

void* virtual_machine::Memory::allocate_memory_pages() 
    { return mmap(NULL, FULL_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); }

void virtual_machine::Memory::deallocate_memory_pages(void* __memory_address) 
    { munmap(__memory_address, FULL_MEMORY_SIZE); }






