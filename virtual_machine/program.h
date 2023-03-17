#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "virtual_machine_definitions.h"

namespace virtual_machine {

    struct Program {

        void* current_instruction, *exit_instruction;
        void* top_stack_memory;
        unsigned short* stack_frame;
        void* memory;

        unsigned short top_stack_memory_addr;

        ~Program(); Program(unsigned short, void*, short); 

        void set_information(void*, short);

        void set_exit_instruction();

        void execute();

    };

}

#endif