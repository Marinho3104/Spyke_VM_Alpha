#pragma once

#ifndef CONVERTOR_H
#define CONVERTOR_H

#include "parser_definitions.h"

namespace parser {

    struct Convertor {

        Implicit_Values_Tracker* implicit_value;
        Ast_Node_Name_Space* global_name_space;
        Code_Information* code_information;

        utils::Linked_List <byte_code::Byte_Code*>* implicit_values_byte_codes;
        utils::Linked_List <byte_code::Byte_Code*>* go_byte_codes;
        utils::Linked_List <byte_code::Byte_Code*>* if_statement_go_end;
        utils::Linked_List <byte_code::Byte_Code_Block*>* blocks;
        utils::Linked_List <byte_code::Byte_Code*>* memory_copy_address_before_go;
        utils::Linked_List <Ast_Node_For*>* for_control_struct_go_back;

        byte_code::Byte_Code_Block* current_block;

        int implicit_values_size, off;

        utils::Linked_List <int>* entry_points;


        bool 
            only_call, // If variable or implicit value call occur, only make load of original value
            force_memory_allocation; // Force a function call to make stack memory allocation of return value

        ~Convertor(); Convertor(Code_Information*, Implicit_Values_Tracker*, Ast_Node_Name_Space*);

        short get_byte_code_length(int);

        short get_byte_code_length(byte_code::Byte_Code*);

        void print(const char*);

        void generate_byte_code();

        void set_initial_block_end();

        void update_entry_points();

        void update_go_byte_code();

        void update_for_control_struct_go_back();

        void update_memory_copy_address_before_go_byte_code();

        void set_block(utils::Linked_List <Ast_Node*>*);

        int add_block();

        byte_code::Compiled_Byte_Code* get_compiled_byte_code();

        // Others //

        void set_variable_declaration_stack_position(Ast_Node_Variable_Declaration*);

        /* For variable declaration with run-time size 
        *
        *   Struct Format:
        *       pointer to first index -- stack allocation -- 2 bytes
        *       all array ( given index size times type size) -- stack allocation 
        *       given array size -- 2 bytes
        */
        void set_variable_declaration_array_size(Ast_Node_Constructor_Call*, int);

        // Byte Code //

        void set_byte_code_stack_memory_allocation(int);

        void set_byte_code_load(int);
        
        void set_byte_code_memory_copy(int);

        void set_byte_code_stack_memory_deallocation(int);

        void set_byte_code_load_inverted(int);
        
        void set_byte_code_go_stack_frame();

        void set_byte_code_memory_copy_stack_frame();

        void set_byte_code_helper(short);

        void set_byte_code_custom_byte_code(short);

        void set_byte_code_load_global(int, byte_code::Byte_Code_Block*);

        void set_byte_code_memory_copy_address();

        void set_byte_code_go(int);

        void set_byte_code_get_address_from_stack();

        void set_byte_code_check(int);

        void set_byte_code_jump(int);

        void set_byte_code_jump_inverted(int);

        void set_byte_code_nop(int);

    };


}

#endif