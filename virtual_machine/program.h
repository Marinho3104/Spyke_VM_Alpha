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

        char custom_byte_code[3];

        bool stop;

        ~Program(); Program(unsigned short, unsigned short, void*, short); 

        void print(const char*);

        void set_information(void*, short);

        void set_exit_instruction();

        void execute_program();
        
        void execute();

        void execute_STACK_MEMORY_ALLOCATION();

        void execute_STACK_MEMORY_DEALLOCATION();

        void execute_LOAD();

        void execute_LOAD_INVERTED();

        void execute_LOAD_GLOBAL();

        void execute_GO();

        void execute_GO_STACK_FRAME();

        void execute_MEMORY_COPY();

        void execute_MEMORY_COPY_ADDRESS();

        void execute_MEMORY_COPY_STACK_FRAME();

        void execute_GET_ADDRESS_FROM_STACK();

        void execute_CUSTOM_BYTE_CODE();

        void execute_binary_arithmetrict_operations(); 

        void execute_binary_comparation_operations(); 

        void execute_CHECK();

        void execute_JUMP();

        void execute_INITIAL_BLOCK_END();

        void execute_JUMP_INVERTED();

    };

}

#endif