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

        void execute(Token*); void skip_block(Token*);

        /* Functions */

        void execute_pre_compiler_instruction();

        bool check_end_block_pre_compiler_instruction();

        /* Define Instructions */

        void execute_pre_compiler_define_instruction();

        Pre_Compiler_Define_Instruction* get_previous_token_same_identifier(char*);

        bool handle_identifier(); void handle_identifier(Token**); 

        /* Include Instructions */
         
        void execute_pre_compiler_include_instruction();

        /* If Define Inctructions */

        void execute_pre_compiler_if_define_instruction();

        /* If Not Define Inctructions */

        void execute_pre_compiler_if_not_define_instruction();

        /* Else And EndIf Instructions */

        void handle_pre_compiler_else_or_end_if(bool);

        void execute_pre_compiler_else_instruction(bool);

        void execute_pre_compiler_end_if_instruction();

    };

}

#endif