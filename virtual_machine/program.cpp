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

virtual_machine::Program::Program(unsigned short __initial_block, unsigned short __entry_point, void* __information, short __information_size) : top_stack_memory_addr(0), exit_instruction(0), stop(0) {

    std::cout << " ---> Program Setup <--- " << std::endl;
    
    top_stack_memory = memory = Memory::allocate_memory_pages();

    set_information(__information, __information_size);

    stack_frame = 
        (unsigned short*) memory + HEAP_MEMORY_SIZE + STACK_MEMORY_SIZE + __information_size;

    current_instruction = memory + __initial_block;

    execute_program();

    stop = 0;

    current_instruction = memory + __entry_point;

    set_exit_instruction();

    std::cout << " ---> Program Setup End <---" << std::endl;

}

void virtual_machine::Program::print(const char* __information) {

    if (0) return;

    std::cout << "--- --- ---\n" << __information << "\n" << std::endl;

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

void virtual_machine::Program::execute_program() {

    unsigned short* _initial_stack_frame = stack_frame;

    while ( current_instruction != exit_instruction && !stop ) {
    
        execute();

        current_instruction = 
            current_instruction + 3;
    
    }

    std::cout << "\n\n--- --- --- --- --- " << std::endl;
    std::cout << "Top stack memory -- " << top_stack_memory_addr << std::endl;
    std::cout << "Initial stack frame position -- " << _initial_stack_frame << std::endl;
    std::cout << "Final stack frame position -- " << stack_frame << std::endl;
    std::cout << "Diff -- " << stack_frame - _initial_stack_frame << std::endl;

}

void virtual_machine::Program::execute() {

_start:

    switch ( *((char*) current_instruction) )
    {

        case STACK_MEMORY_ALLOCATION: print("STACK_MEMORY_ALLOCATION"); execute_STACK_MEMORY_ALLOCATION(); break;
        case STACK_MEMORY_DEALLOCATION: print("STACK_MEMORY_DEALLOCATION"); execute_STACK_MEMORY_DEALLOCATION(); break;
        case LOAD: print("LOAD"); execute_LOAD(); break;
        case LOAD_INVERTED: print("LOAD_INVERTED"); execute_LOAD_INVERTED(); break;
        case LOAD_GLOBAL: print("LOAD_GLOBAL"); execute_LOAD_GLOBAL(); break;
        case GO: print("GO"); execute_GO(); goto _start; break;
        case GO_STACK_FRAME: print("GO_STACK_FRAME"); execute_GO_STACK_FRAME(); goto _start; break;
        case MEMORY_COPY: print("MEMORY_COPY"); execute_MEMORY_COPY(); break;
        case MEMORY_COPY_ADDRESS: print("MEMORY_COPY_ADDRESS"); execute_MEMORY_COPY_ADDRESS(); break;
        case MEMORY_COPY_STACK_FRAME: print("MEMORY_COPY_STACK_FRAME"); execute_MEMORY_COPY_STACK_FRAME(); break;
        case GET_ADDRESS_FROM_STACK: print("GET_ADDRESS_FROM_STACK"); execute_GET_ADDRESS_FROM_STACK(); break;
        case CUSTOM_BYTE_CODE: print("CUSTOM_BYTE_CODE"); execute_CUSTOM_BYTE_CODE(); break;
        case NOP: {std::cout << "Nop -> " << *( (short*) (current_instruction + 1) ) << std::endl; std::string _; std::cin >> _; break;}
        case HELPER: break;
        case BINARY_ADD: case BINARY_SUB: case BINARY_MUL: case BINARY_DIV: case BINARY_MOD: case BINARY_INCREMENT: case BINARY_DECREMENT:
            print("BINARY ARITHMETRIC OPERATIONS"); execute_binary_arithmetrict_operations(); break;
        case BINARY_EQUAL_TO: case BINARY_NOT_EQUAL_TO: case BINARY_GREATER_THAN: case BINARY_LESS_THAN: case BINARY_GREATER_THAN_EQUAL_TO: 
            case BINARY_LESS_THAN_EQUAL_TO: print("BINARY COMPARATIONS"); execute_binary_comparation_operations(); break;
        case CHECK: print("CHECK"); execute_CHECK(); break;
        case JUMP: print("JUMP"); execute_JUMP(); break;
        case INITIAL_BLOCK_END: stop = 1; break;
        case JUMP_INVERTED: print("JUMP_INVERTED"); execute_JUMP_INVERTED(); break;
        default: std::cout << "Code not defined " << (int) *((char*) current_instruction) << std::endl; exit(1); break;
    }


}

void virtual_machine::Program::execute_STACK_MEMORY_ALLOCATION() {

    if (
      *( (char*) ( current_instruction - 3 ) ) == HELPER
    ) {

        switch ( *( (short*) ( current_instruction - 5 ) ) )
        {
        case STACK_MEMORY_ALLOCATION_MODE_MUL: 
            *( (short*) ( current_instruction + 1 ) ) *= *( (short*) ( current_instruction - 2 ) ); break;
        
        default: std::cout << "Error stack memory allocation -- mode" << std::endl; exit(1); break;
        }

    }

    std::cout << "Allocation size -> " << *( (short*) ( current_instruction + 1 ) ) << std::endl;

    top_stack_memory = top_stack_memory + *( (short*) ( current_instruction + 1 ) ); 
    
    top_stack_memory_addr += *( (short*) ( current_instruction + 1 ) );

}

void virtual_machine::Program::execute_STACK_MEMORY_DEALLOCATION() {

    top_stack_memory = top_stack_memory - *( (short*) ( current_instruction + 1 ) ); 
    
    top_stack_memory_addr -= *( (short*) ( current_instruction + 1 ) );

    std::cout << "Deallocation size -> " << *( (short*) ( current_instruction + 1 ) )  << std::endl;

}

void virtual_machine::Program::execute_LOAD() { 
    
    if (
        *( (char*) ( current_instruction - 3 ) ) == HELPER
    ) {
        std::cout << "Value before -> " << *( (short*) ( current_instruction + 1 ) ) << std::endl;

        int _helper_count = 0;

        void* _current_stack_cpy = 
            current_instruction;

        while( *( (char*) ( _current_stack_cpy - 3 ) ) == HELPER ) { _current_stack_cpy = _current_stack_cpy - 6; _helper_count ++; }

        std::cout << _helper_count << std::endl;

        for (; _helper_count > 0; _helper_count --, _current_stack_cpy = _current_stack_cpy + 6)

            switch ( *( (char*) ( _current_stack_cpy + 1 ) ) )
            {
            case LOAD_MODE_ADD:
                *( (short*) ( current_instruction + 1 ) ) += *( (short*) ( _current_stack_cpy + 4 ) ); break;
            case LOAD_MODE_MUL: 
                *( (short*) ( current_instruction + 1 ) ) *= *( (short*) ( _current_stack_cpy + 4 ) ); break;
            
            case LOAD_STACK_FRAME_MODE_ADD:

                *( (short*) ( current_instruction + 1 ) ) += 
                    *((short*) (memory + *(--stack_frame))) * *( (short*) ( _current_stack_cpy + 4 ) ); break;
                
            default: std::cout << "Error load -- mode " << (int) *( (char*) ( _current_stack_cpy ) ) << std::endl; exit(1); break;
            }

    }

    *(stack_frame++) = top_stack_memory_addr - *( (short*) ( current_instruction + 1 ) ); 
    
    std::cout << "Argument given -> " << *( (short*) ( current_instruction + 1 ) ) << std::endl;
    std::cout << "Loaded value -> " << (short) *((short*) (memory + *(stack_frame - 1))) << std::endl; 
    
}

void virtual_machine::Program::execute_LOAD_INVERTED() {

    if (
      *( (char*) ( current_instruction - 3 ) ) == HELPER
    ) {

        switch ( *( (short*) ( current_instruction - 5 ) ) )
        {
        case LOAD_INVERTED_MODE_MUL: 
            
            *( (short*) ( current_instruction + 1 ) ) *= *( (short*) ( current_instruction - 2 ) ); break;
        
        default: std::cout << "Error stack memory allocation -- mode" << std::endl; exit(1); break;
        }

    }

    *(stack_frame++) = top_stack_memory_addr + *( (short*) ( current_instruction + 1 ) );

    std::cout << "Off -> " <<  *( (short*) ( current_instruction + 1 ) ) << std::endl;

    std::cout << "Loaded value -> " << (int) *((int*) (memory + *(stack_frame - 1))) << std::endl;

}

void virtual_machine::Program::execute_LOAD_GLOBAL() { *(stack_frame++) = *( (short*) ( current_instruction + 1 ) ); std::cout << "Loaded value -> " << (int) *((int*) (memory + *(stack_frame - 1))) << std::endl; }

void virtual_machine::Program::execute_GO() { current_instruction = memory + *( (short*) ( current_instruction + 1 ) ); }

void virtual_machine::Program::execute_GO_STACK_FRAME() {

    current_instruction = memory + *((short*) (memory + *(--stack_frame))) + *( (short*) ( current_instruction + 1 ) ) * 3;

}

void virtual_machine::Program::execute_MEMORY_COPY() {

    std::cout << "Copy size -> " << *( (short*) ( current_instruction + 1 ) ) << std::endl;

    std::cout << "Off -> " << *( stack_frame - 1) << std::endl;
    std::cout << "Off -> " << *( stack_frame - 2) << std::endl;

    std::cout << "Data before -> " << *((short *) (memory + *( stack_frame - 1))) << std::endl;
    std::cout << "Data copy -> " << *((short *) (memory + *( stack_frame - 2))) << std::endl;

    memcpy(
        memory + *( stack_frame - 1),
        memory + *( stack_frame - 2),
        *( (short*) ( current_instruction + 1 ) )
    ); 

    std::cout << "Data after -> " << *((short *) (memory + *( stack_frame - 1))) << std::endl;

    stack_frame -= 2;

    std::string ola;

    // if (*( (short*) ( current_instruction + 1 ) ) == 2) std::cin >> ola;

}

void virtual_machine::Program::execute_MEMORY_COPY_ADDRESS() {

    memcpy(
        memory + *(-- stack_frame),
        current_instruction + 1,
        2
    );

}

void virtual_machine::Program::execute_MEMORY_COPY_STACK_FRAME() {

    memcpy(
        memory + *( stack_frame - 1),
        stack_frame - 2,
        2
    ); stack_frame -= 2;

}

void virtual_machine::Program::execute_GET_ADDRESS_FROM_STACK() { *( stack_frame - 1 ) = *( (short*) ( memory + *( stack_frame - 1 ) ) ); }

void virtual_machine::Program::execute_CUSTOM_BYTE_CODE() {

    memcpy(
        custom_byte_code,
        current_instruction,
        3
    );

    if ( *( (short*) ( current_instruction + 1 ) ) ) {

        *((char*) current_instruction) = *((char*) ( current_instruction + 1 ));

        *( (short*) ( current_instruction + 1 ) ) = *((short*) (memory + *(-- stack_frame)));

    }

    else {

        *( (short*) ( current_instruction + 1 ) ) = *((short*) (memory + *(-- stack_frame)));
        *((char*) current_instruction) = *((char*) (memory + *(-- stack_frame)));

    }

    execute();

    memcpy(
        current_instruction,
        custom_byte_code,
        3
    );

}

void virtual_machine::Program::execute_binary_arithmetrict_operations() {

    switch( *((char*) current_instruction) ) {

        case BINARY_ADD: case BINARY_SUB:

            binary_arithmetrict_operations_0(
                memory + *(stack_frame - 2),
                memory + *(stack_frame - 1),
                *((short*) ( current_instruction + 1 ) ),
                *((char*) current_instruction)
            ); stack_frame -= 2; break;

        case BINARY_INCREMENT: case BINARY_DECREMENT: 

                binary_arithmetrict_operations_0(
                    memory + *(stack_frame - 1),
                    0,
                    *((short*) ( current_instruction + 1 ) ),
                    *((char*) current_instruction)   
                ); stack_frame -= 1; break;

        case BINARY_MUL: 
        
            switch ( *((short*) ( current_instruction + 1 ) ) )
            {
            case 1: 
            
                *(   (bool*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (bool*) (memory + *(stack_frame - 2)) ) * *( (bool*) (memory + *(stack_frame - 1)) );

                break;

            case 2:

                *(   (short*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (short*) (memory + *(stack_frame - 2)) ) * *( (short*) (memory + *(stack_frame - 1)) );

                break;
                

            case 4: 

                *(   (int*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (int*) (memory + *(stack_frame - 2)) ) * *( (int*) (memory + *(stack_frame - 1)) );

                break;
            
            default: std::cout << "Not available size" << std::endl; exit(1); break;

            } stack_frame -= 2; break;

        case BINARY_DIV: 
        
            switch ( *((short*) ( current_instruction + 1 ) ) )
            {
            case 1: 
            
                *(   (bool*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (bool*) (memory + *(stack_frame - 2)) ) / *( (bool*) (memory + *(stack_frame - 1)) );

                break;

            case 2:

                *(   (short*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (short*) (memory + *(stack_frame - 2)) ) / *( (short*) (memory + *(stack_frame - 1)) );

                break;
                

            case 4: 

                *(   (int*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (int*) (memory + *(stack_frame - 2)) ) / *( (int*) (memory + *(stack_frame - 1)) );

                break;
            
            default: std::cout << "Not available size" << std::endl; exit(1); break;

            } stack_frame -= 2; break;

        case BINARY_MOD:

            switch ( *((short*) ( current_instruction + 1 ) ) )
            {
            case 1: 
            
                *(   (bool*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (bool*) (memory + *(stack_frame - 2)) ) % *( (bool*) (memory + *(stack_frame - 1)) );

                break;

            case 2:

                *(   (short*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (short*) (memory + *(stack_frame - 2)) ) % *( (short*) (memory + *(stack_frame - 1)) );

                break;
                

            case 4: 

                *(   (int*)
                    (memory + *(stack_frame - 2))
                ) = 
                    *( (int*) (memory + *(stack_frame - 2)) ) % *( (int*) (memory + *(stack_frame - 1)) );

                break;
            
            default: std::cout << "Not available size" << std::endl; exit(1); break;

            } stack_frame -= 2; break;

    }

}

void virtual_machine::Program::execute_binary_comparation_operations() {

    binary_comparation_operations(
        memory + *(stack_frame - 2),
        memory + *(stack_frame - 1),
        memory + *(stack_frame - 3),
        *((short*) ( current_instruction + 1 ) ),
        *((char*) current_instruction)        
    ); stack_frame -= 3;

} 

void virtual_machine::Program::execute_CHECK() {

    std::cout << "Result -> " << *((bool*) (memory + *(stack_frame - 1))) << std::endl;

    if (
        *((bool*) (memory + *(--stack_frame)))
    ) current_instruction = current_instruction + (*( ((short*) ( current_instruction + 1 )) ) ) * 3;

}

void virtual_machine::Program::execute_JUMP() {

    std::cout << "Argument -> " << (*( (short*) ( current_instruction + 1 ) ) - 1) << std::endl;

    current_instruction = 
        current_instruction + (*( (short*) ( current_instruction + 1 ) ) - 1) * 3;

}

void virtual_machine::Program::execute_INITIAL_BLOCK_END() {

}

void virtual_machine::Program::execute_JUMP_INVERTED() {

    current_instruction = 
        current_instruction - (*( (short*) ( current_instruction + 1 ) ) + 1) * 3;

}


