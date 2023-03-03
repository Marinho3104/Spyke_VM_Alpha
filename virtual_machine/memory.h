#pragma once

#ifndef MEMORY_H
#define MEMORY_H

namespace virtual_machine {

    struct Memory {

        static void* allocate_memory_pages();

        static void deallocate_memory_pages(void*);

    };

}

#endif