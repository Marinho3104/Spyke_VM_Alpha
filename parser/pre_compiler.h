#pragma once

#ifndef PARSER_PRE_COMPILER_H
#define PARSER_PRE_COMPILER_H

#include "parser_definitions.h"

namespace parser {

    struct Pre_Compiler {

        utils::Linked_List <Token*>* tokens_collection;
        int tokens_position;

        utils::Linked_List <Pre_Compiler_Define_Instruction*>* active_define_instructions;

        Code_Information* code_information;

        ~Pre_Compiler(); Pre_Compiler(Code_Information*, utils::Linked_List <Token*>*);

        void execute();

        /* Functions */

        void execute_pre_compiler_instruction();

        /* Define Instructions */

        void execute_pre_compiler_define_instruction();

        Pre_Compiler_Define_Instruction* get_previous_token_same_identifier(char*);

        void handle_identifier();

    };

}

#endif