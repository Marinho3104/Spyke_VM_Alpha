#pragma once

#ifndef CONVERTOR_H
#define CONVERTOR_H

#include "parser_definitions.h"

namespace parser {

    struct Convertor {

        utils::Linked_List <Token*>* implicit_value;
        Ast_Node_Name_Space* global_name_space;
        Code_Information* code_information;

        utils::Linked_List <byte_code::Byte_Code_Block*>* blocks;

        byte_code::Byte_Code_Block* current_block;

        ~Convertor(); Convertor(Code_Information*, utils::Linked_List <Token*>*, Ast_Node_Name_Space*);

        void print(const char*);

        void generate_byte_code();

        void set_block(utils::Linked_List <Ast_Node*>*, int);

        void set_block(utils::Linked_List <Ast_Node*>*);

        int add_block();

        byte_code::Compiled_Byte_Code* get_compiled_byte_code();

    };


}

#endif