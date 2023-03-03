#include "program.h"

#include "program_definitions.h"
#include "byte_code.h"
#include "opcodes.h"
#include "memory.h"

#include <iostream>
#include <string.h>

virtual_machine::Program::~Program() { Memory::deallocate_memory_pages(memory); }

virtual_machine::Program::Program(short __entry_point, void* __information, short __information_size) : top_stack_memory_addr(0) {

    std::cout << " ---> Program Setup <--- " << std::endl;
    
    top_stack_memory = memory = Memory::allocate_memory_pages();

    set_information(__information, __information_size);

    current_instruction = memory + __entry_point;

    stack_frame = 
        (short*) memory + HEAP_MEMORY_SIZE + STACK_MEMORY_SIZE + __information_size;

    std::cout << " ---> Program Setup End <---" << std::endl;

}

void virtual_machine::Program::set_information(void* __information, short __information_size) {

    memcpy(
        memory + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE,
        __information,
        __information_size
    );

}

void virtual_machine::Program::execute() {

    char _code;
    short _argument;

    do {

        _code = *((char*) current_instruction);
        _argument = *((unsigned short*) (current_instruction + 1));

        std::cout << "Code -> " << (int) _code << std::endl;
        std::cout << "Argument -> " << _argument << std::endl;
        std::cout << "--- --- --- --- --- " << std::endl;

        switch (_code)
        {
            case STACK_MEMORY_ALLOCATION:

                top_stack_memory =
                    top_stack_memory + _argument;

                break;

            case STACK_MEMORY_DEALLOCATION:

                top_stack_memory =
                    top_stack_memory - _argument;                

                break;

            case LOAD:

                *(stack_frame++) = 
                    top_stack_memory_addr - _argument;

                break; 

            case LOAD_GLOBAL:

                *(stack_frame++) = 
                    _argument;

                break;   

            case CALL:

                current_instruction = 
                    memory + _argument;
        
                break;

        case CLOSE_STACK_FRAME: goto out; break;
        default: break;
        }

        current_instruction = 
            current_instruction + 3;

    } while (top_stack_memory != memory);

out:;

}

