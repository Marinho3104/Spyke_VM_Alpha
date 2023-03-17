#include "convertor.h"

#include "program_definitions.h"
#include "convertor_ast.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "ast_nodes.h"
#include "byte_code.h"
#include "opcodes.h"

#include <iostream>




parser::Convertor::~Convertor() { delete blocks; delete implicit_values_byte_codes; delete entry_points; delete call_byte_codes; delete memory_copy_stack_frame_before_go; }

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
        call_byte_codes = new utils::Linked_List <byte_code::Byte_Code*>(0); 
        memory_copy_stack_frame_before_go = new utils::Linked_List <byte_code::Byte_Code*>(0);
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

    update_call_byte_code(); 

    update_memory_copy_stack_frame_before_go_byte_code(); 

    update_entry_points();

    update_for_control_struct_go_back();

}

void parser::Convertor::update_entry_points() {

    short _current_length = 0;

    for (int _ = 0; _ < blocks->count; _++) {

        if (blocks->operator[](_)->entry_point)

            entry_points->add(
                _current_length + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + implicit_values_size
            );

        _current_length += blocks->operator[](_)->block->count * 3;

    }

}

void parser::Convertor::update_call_byte_code() {

    for (int _ = 0; _ < call_byte_codes->count; _++) {

        call_byte_codes->getDataLinkedList(_)->object->argument = 
            get_byte_code_length(
                call_byte_codes->operator[](_)->argument
            ) + STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + implicit_values_size;

    }

}

void parser::Convertor::update_memory_copy_stack_frame_before_go_byte_code() {

    for (int _ = 0; _ < memory_copy_stack_frame_before_go->count; _++) {

        std::cout << "Loop " << std::endl;

        memory_copy_stack_frame_before_go->getDataLinkedList(_)->object->argument = 
            get_byte_code_length(
                memory_copy_stack_frame_before_go->operator[](_)
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


