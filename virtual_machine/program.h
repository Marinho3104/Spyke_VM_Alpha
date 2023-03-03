#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "virtual_machine_definitions.h"

namespace virtual_machine {

    struct Program {

        void* current_instruction;
        void* top_stack_memory;
        short* stack_frame;
        void* memory;

        short top_stack_memory_addr;

        ~Program(); Program(short, void*, short); 

        void set_information(void*, short);

        void execute();

    };

}

#endif