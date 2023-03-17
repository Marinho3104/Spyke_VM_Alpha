#include "program.h"

#include "program_definitions.h"
#include "program_helper.h"
#include "byte_code.h"
#include "opcodes.h"
#include "memory.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

virtual_machine::Program::~Program() { Memory::deallocate_memory_pages(memory); }

virtual_machine::Program::Program(unsigned short __entry_point, void* __information, short __information_size) : top_stack_memory_addr(0) {

    std::cout << " ---> Program Setup <--- " << std::endl;
    
    top_stack_memory = memory = Memory::allocate_memory_pages();

    set_information(__information, __information_size);

    current_instruction = memory + __entry_point;

    set_exit_instruction();

    stack_frame = 
        (unsigned short*) memory + HEAP_MEMORY_SIZE + STACK_MEMORY_SIZE + __information_size;

    std::cout << " ---> Program Setup End <---" << std::endl;

}

void virtual_machine::Program::set_information(void* __information, short __information_size) {

    memcpy(
        memory + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE,
        __information,
        __information_size
    );

}

void virtual_machine::Program::set_exit_instruction() {

    exit_instruction = current_instruction;

    while(*((char*) exit_instruction) != GO_STACK_FRAME) 

        exit_instruction = 
            exit_instruction + 3;

}

void virtual_machine::Program::execute() {

    char _code;
    unsigned short _argument;

    unsigned short* _stack_frame = 
        stack_frame;

    while (current_instruction != exit_instruction) {

        _code = *((char*) current_instruction);
        _argument = *((unsigned short*) (current_instruction + 1));

    repeat_execution:


        std::cout << "Current instruction -> " << current_instruction << std::endl;
        std::cout << "Exit instruction -> " << exit_instruction << std::endl;
        std::cout << "Inicial stack frame -> " << _stack_frame << std::endl;
        std::cout << "Current stack frame -> " << stack_frame << std::endl;
        std::cout << "Code -> " << (int) _code << std::endl;
        std::cout << "Argument -> " << _argument << std::endl;
        std::cout << "--- --- --- --- --- " << std::endl;

        switch (_code)
        {
            case STACK_MEMORY_ALLOCATION: top_stack_memory = top_stack_memory + _argument; top_stack_memory_addr += _argument; break;
            case STACK_MEMORY_DEALLOCATION: top_stack_memory = top_stack_memory - _argument; top_stack_memory_addr -= _argument; break;
            case LOAD: 
            *(stack_frame++) = top_stack_memory_addr - _argument; 
            std::cout << "Top stack memory -> " << top_stack_memory_addr << std::endl;
            std::cout << "Off -> " << (unsigned short) *(stack_frame - 1) << std::endl;
            std::cout << "Loaded value -> " << (int) *((int*) (memory + *(stack_frame - 1))) << std::endl; 
            break; 
            case LOAD_INVERTED: *(stack_frame++) = top_stack_memory_addr + _argument; std::cout << "Loaded value -> " << *((short*) (memory + *(stack_frame - 1))) << std::endl; break; 
            case LOAD_GLOBAL: *(stack_frame++) = _argument; std::cout << "Loaded value -> " << *((short*) (memory + *(stack_frame - 1))) << std::endl; break;   
            case GO: current_instruction = memory + _argument - 3; std::cout << "\t\t\tCall function " << std::endl; break;
            case GO_STACK_FRAME: current_instruction = memory - 3 + *((short*) (memory + *(--stack_frame))) + _argument * 3; std::cout << "\t\t\t\tBackup " << std::endl; break;
        
            case MEMORY_COPY: 

                std::cout << "Before -> " << *((int*)(memory + *(stack_frame - 1))) << " " << *(stack_frame - 1) << std::endl;
                std::cout << "Before -> " << *((int*)(memory + *(stack_frame - 2))) << " " << *(stack_frame - 2) << std::endl;

                memcpy(
                    memory + *(stack_frame - 1),
                    memory + *(stack_frame - 2),
                    _argument
                );

                std::cout << "After -> " << *((int*)(memory + *(stack_frame - 1))) << std::endl;
                std::cout << "After -> " << *((int*)(memory + *(stack_frame - 2))) << std::endl;

                stack_frame -= 2;

                break;

            case MEMORY_COPY_ADDRESS: 
            
                memcpy(
                    memory + *(stack_frame - 1),
                    &_argument,
                    2
                );

                stack_frame --;


                break;

            case MEMORY_COPY_STACK_FRAME:

                memcpy(
                    memory + *(stack_frame - 1),
                    stack_frame - 2,
                    2
                );

                stack_frame -= 2;

                break;

            case GET_ADDRESS_FROM_STACK:

                *(stack_frame - 1) = 
                    *( (short*) (memory + *(stack_frame - 1)) );

                break;

            case CUSTOM_BYTE_CODE:

                _code = *((char*) (memory + *(stack_frame - 2)));
                _argument = *((short*) (memory + *(stack_frame - 1)));

                stack_frame -= 2;

                goto repeat_execution; break;

            case NOP: std::cout << "\t\t\t\tNOPPPP " << std::endl; break; 
            case BINARY_ADD: case BINARY_SUB: case BINARY_MUL: case BINARY_DIV: case BINARY_MOD:

                binary_arithmetrict_operations(
                    memory + *(stack_frame - 2),
                    memory + *(stack_frame - 1),
                    _argument,
                    _code
                ); stack_frame -= 2; break;

            case BINARY_EQUAL_TO: case BINARY_NOT_EQUAL_TO: 

                binary_comparation_operations(
                    memory + *(stack_frame - 2),
                    memory + *(stack_frame - 1),
                    memory + *(stack_frame - 3),
                    _argument,
                    _code        
                ); stack_frame -= 3; break;
            
            case CHECK:

                if (
                    ! *((bool*) (memory + *(--stack_frame)))
                ) current_instruction = current_instruction + (_argument) * 3;

                std::cout << "Check with value -> " << (int) *((char*) (memory + *(stack_frame - 1))) << std::endl;

                exit(1);

            case CHECK_NOT:

                if (
                    *((bool*) (memory + *(--stack_frame)))
                ) current_instruction = current_instruction + _argument * 3;

                break;

            case JUMP:

                current_instruction = 
                    current_instruction + _argument * 3;

            default: break;
        }

        current_instruction = 
            current_instruction + 3;

        std::cout << "\n" << std::endl;

        // sleep(1);

    }

out:;

    std::cout << "Inicial stack frame position -> " << _stack_frame << std::endl;
    std::cout << "Final stack frame position -> " << stack_frame << std::endl;

    std::cout << "\n Top Stack memory -> " << top_stack_memory_addr << std::endl;

}

