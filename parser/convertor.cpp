#include "convertor.h"

#include "program_definitions.h"
#include "convertor_ast.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "ast_nodes.h"
#include "byte_code.h"
#include "opcodes.h"

#include <iostream>




parser::Convertor::~Convertor() { delete blocks; delete implicit_values_byte_codes; delete entry_points; delete go_byte_codes; delete memory_copy_address_before_go; delete for_control_struct_go_back; delete if_statement_go_end; }

parser::Convertor::Convertor(Code_Information* __code_informaiton, parser::Implicit_Values_Tracker* __implicit_value, Ast_Node_Name_Space* __global_name_space)
    : code_information(__code_informaiton), implicit_value(__implicit_value), global_name_space(__global_name_space), current_block(0), implicit_values_size(0), off(0), only_call(0), force_memory_allocation(0) { 
        
        for (int _ = 0; _ < implicit_value->implicit_value_tokens->count; _++)

            implicit_values_size +=     
                get_implicit_value_type_size(
                    implicit_value->implicit_value_tokens->operator[](
                        _
                    )
                );

        blocks = new utils::Linked_List <byte_code::Byte_Code_Block*>(); 
        
        implicit_values_byte_codes = new utils::Linked_List <byte_code::Byte_Code*>(0); 
        go_byte_codes = new utils::Linked_List <byte_code::Byte_Code*>(0); 
        if_statement_go_end = new utils::Linked_List <byte_code::Byte_Code*>(0); 
        memory_copy_address_before_go = new utils::Linked_List <byte_code::Byte_Code*>(0);
        for_control_struct_go_back = new utils::Linked_List <parser::Ast_Node_For*>(0);

        entry_points = new utils::Linked_List <int>();
        
        generate_byte_code(); 
        
    }

short parser::Convertor::get_byte_code_length(int __position) {

    short _length = 0;

    for (int _ = 0; _ < __position; _++)

        _length += blocks->operator[](_)->block->count * 3;

    return _length;

}

short parser::Convertor::get_byte_code_length(byte_code::Byte_Code* __byte_code) {

    short _length = 0;

    for (int _ = 0; _ < blocks->count; _++)

        for (int __ = 0; __ < blocks->operator[](_)->block->count; __++) {

            if (blocks->operator[](_)->block->operator[](__) == __byte_code) goto out;

            _length += 3;

        }

out:;

    return _length;

}

void parser::Convertor::print(const char* __information) { if (0) return; std::cout << "\t" << __information << "\n" << std::endl; }

void parser::Convertor::generate_byte_code() { 

    add_block();

    current_block = 
        blocks->operator[](0);
    
    set_block(global_name_space->declarations);

    set_initial_block_end();

    update_go_byte_code(); 

    update_memory_copy_address_before_go_byte_code(); 

    update_entry_points();

    update_for_control_struct_go_back();

}

void parser::Convertor::set_initial_block_end() {

    blocks->operator[](0)->block->add(
        byte_code::Byte_Code::generate(
            INITIAL_BLOCK_END,
            0
        )
    );

}

void parser::Convertor::update_entry_points() {

    short _current_length = 0;

    for (int _ = 0; _ < blocks->count; _++) {

        if ( ! _ || blocks->operator[](_)->entry_point)

            entry_points->add(
                _current_length + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + implicit_values_size
            );

        _current_length += blocks->operator[](_)->block->count * 3;

    }

}

void parser::Convertor::update_go_byte_code() {

    for (int _ = 0; _ < go_byte_codes->count; _++) {

        go_byte_codes->getDataLinkedList(_)->object->argument = 
            get_byte_code_length(
                go_byte_codes->operator[](_)->argument
            ) + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + implicit_values_size;

    }

}

void parser::Convertor::update_memory_copy_address_before_go_byte_code() {

    for (int _ = 0; _ < memory_copy_address_before_go->count; _++) {

        memory_copy_address_before_go->getDataLinkedList(_)->object->argument = 
            get_byte_code_length(
                memory_copy_address_before_go->operator[](_)
            ) + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + implicit_values_size + 6; // Byte code after MEMORY_COPY

    }

}

void parser::Convertor::update_for_control_struct_go_back() {

    for (int _ = 0; _ < for_control_struct_go_back->count; _++)

        for_control_struct_go_back->operator[](_)->go_back->argument += 
            3 * for_control_struct_go_back->operator[](_)->variable_declarations_instructions_count;


}

void parser::Convertor::set_block(utils::Linked_List <Ast_Node*>* __asts) {

    for (int _ = 0; _ < __asts->count; _++) set_byte_code_of_ast(this, __asts->operator[](_));

}

int parser::Convertor::add_block() {

    byte_code::Byte_Code_Block* _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_byte_code_block) byte_code::Byte_Code_Block();
    
    return blocks->add(_byte_code_block);

}

byte_code::Compiled_Byte_Code* parser::Convertor::get_compiled_byte_code() {

    byte_code::Compiled_Byte_Code* _compiled_byte_code = 
        new byte_code::Compiled_Byte_Code(
            blocks
        );

    _compiled_byte_code->set_implicit_values(
        implicit_value->implicit_value_tokens, implicit_values_size
    );

    return 
        _compiled_byte_code;

}

// Others //

void parser::Convertor::set_variable_declaration_stack_position(Ast_Node_Variable_Declaration* __variable_declaration_node) 
    { __variable_declaration_node->stack_position = current_block->current_stack_size; }

void parser::Convertor::set_variable_declaration_array_size(parser::Ast_Node_Constructor_Call* __constructor_call_node, int __s) {

    // Initial pointer is already allocated so
    // first set the value of that pointer to the first index

    // Load first index position
    set_byte_code_load(0);

    // Load initial pointer
    set_byte_code_load(
        2 // address size
    );

    // Make a copy of the address loaded into given address
    set_byte_code_memory_copy_stack_frame();


    // Execute the run-time variable ( array size )

    bool _prev_force_allocation_value = force_memory_allocation; force_memory_allocation = 1;
    bool _prev_call_value = only_call; only_call = 1;

    set_byte_code_of_ast(
        this,
        __constructor_call_node
    );

    force_memory_allocation = _prev_force_allocation_value;
    only_call = _prev_call_value;

    // Need an extra load of run-time size to execute the memory copy of run-time value into the end of array size
    set_byte_code_load(
        2
    ); 

    
    // Makes stack memory allocation
    // Needs to stack allocate ( variable size times array variable type )

    // Set helper to notify the VM to before allocate memory 
    // multiply the argument for helper given argument

    // Notify the VM to multiply the values
    set_byte_code_helper(
        STACK_MEMORY_ALLOCATION_MODE_MUL
    );

    // Helper argument
    set_byte_code_helper(
        __s
    );

    // Custom byte to execute the memory allocation with run-time array size
    set_byte_code_custom_byte_code(
        STACK_MEMORY_ALLOCATION
    );

    // Load of the final 2 bytes ( new address of run-time value )
    set_byte_code_load(
        2
    );

    // Makes the copy
    set_byte_code_memory_copy(
        2 // address size
    );

    // Here we add the compile time stack position of the array size
    current_block->custom_sizes->add(
        current_block->current_stack_size - 2 // Current address minus 2, which is the run-time array size position
    );

    // Just add variable size for forward manipulation
    current_block->custom_sizes_h->add(
        __s
    );

}


// Byte Code //

void parser::Convertor::set_byte_code_stack_memory_allocation(int __s) {

    // Add byte code STACK_MEMORY_ALLOCATION
    current_block->block->add(

        byte_code::Byte_Code::generate(
            
            STACK_MEMORY_ALLOCATION,
            __s

        )

    );

    // Update current_stack_size
    current_block->current_stack_size +=
        __s;

}

void parser::Convertor::set_byte_code_load(int __s) {

    // load byte code
    current_block->block->add(

        current_block->get_load_byte_code( __s )

    );

}

void parser::Convertor::set_byte_code_memory_copy(int __s) {

    // Memory copy byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(
            
            MEMORY_COPY,
            __s

        )

    );

}

void parser::Convertor::set_byte_code_stack_memory_deallocation(int __s) {

    // stack memory deallocation byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(
            
            STACK_MEMORY_DEALLOCATION,
            __s

        )

    );

    // Update current_stack_size
    current_block->current_stack_size -=
        __s;

}

void parser::Convertor::set_byte_code_load_inverted(int __s) {

    // Load inverted byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            LOAD_INVERTED,
            __s

        )

    );

}

void parser::Convertor::set_byte_code_go_stack_frame() {

    // Go Stack Frame byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            GO_STACK_FRAME,
            0

        )

    );

}

void parser::Convertor::set_byte_code_memory_copy_stack_frame() {

    // Memory copy stack frame byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            MEMORY_COPY_STACK_FRAME,
            0

        )

    );

}

void parser::Convertor::set_byte_code_helper(short __arg) {

    // Helper byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            HELPER,
            __arg

        )

    );

}

void parser::Convertor::set_byte_code_custom_byte_code(short __arg) {

    // custom byte code byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            CUSTOM_BYTE_CODE,
            __arg

        )

    );

}

void parser::Convertor::set_byte_code_load_global(int __s, byte_code::Byte_Code_Block* __glb_block) {
    
    // global load byte code
    current_block->block->add(

        current_block->get_load_global_byte_code( __s, __glb_block )

    );


}

void parser::Convertor::set_byte_code_memory_copy_address() {

    // Memory copy address
    current_block->block->add(

        byte_code::Byte_Code::generate(

            MEMORY_COPY_ADDRESS,
            0

        )

    );

    // Because at this point we dont know if will be added more byte code before this position
    // Can´t assign now the position
    // So this byte code is added to a linked list to assign the correct position after all byte code is done
    memory_copy_address_before_go->add(
        current_block->block->last->object
    );

}

void parser::Convertor::set_byte_code_go(int _body_position) {

    // Go byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            GO,
            _body_position

        )

    );

    // for the some reason above, the byte code is added to an array to be set the real address
    // after all byte code is generated
    // Only difference is that here already put some in argument which is the body position of the function we want the 
    // address from
    go_byte_codes->add(
        current_block->block->last->object
    ); 

}

void parser::Convertor::set_byte_code_get_address_from_stack() {

    // get address from stack byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            GET_ADDRESS_FROM_STACK,
            0

        )

    );

}

void parser::Convertor::set_byte_code_check( int __jmp_amount ) {

    // check byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            CHECK,
            __jmp_amount

        )

    );

}

void parser::Convertor::set_byte_code_jump( int __jmp_amount) {

    // jump byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            JUMP,
            __jmp_amount

        )

    );

}

void parser::Convertor::set_byte_code_jump_inverted( int __jmp_amount) {
    
    // jump inverted byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            JUMP_INVERTED,
            __jmp_amount

        )

    );

}

void parser::Convertor::set_byte_code_nop(int __) {
    
    // nop byte code
    current_block->block->add(

        byte_code::Byte_Code::generate(

            NOP,
            __

        )

    );


}



