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
        utils::Linked_List <byte_code::Byte_Code*>* call_byte_codes;
        utils::Linked_List <byte_code::Byte_Code_Block*>* blocks;

        byte_code::Byte_Code_Block* current_block;

        int implicit_values_size;

        utils::Linked_List <int>* entry_points;

        ~Convertor(); Convertor(Code_Information*, Implicit_Values_Tracker*, Ast_Node_Name_Space*);

        short get_byte_code_length(int);

        void print(const char*);

        void generate_byte_code();

        void update_entry_points();

        void update_call_byte_code();

        void set_block(utils::Linked_List <Ast_Node*>*);

        int add_block();

        byte_code::Compiled_Byte_Code* get_compiled_byte_code();

    };


}

#endif