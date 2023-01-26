#pragma once

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

#include "parser_definitions.h"
#include <stddef.h>

namespace parser {

    struct Tokenizer {

        char* code, *code_to_free, *collumn_inicial_address;
        utils::Linked_List <Token*>* tokens_collection;
        Code_Information* code_information;
        size_t current_line;


        ~Tokenizer(); Tokenizer(Code_Information*, char*);

        void set_tokens();

        void print_tokens();

        /* Functions */

        void handle_new_token();

        bool handle_token_symbol(bool);

        bool handle_token_keyword();

        bool handle_token_implicit_value();

        void handle_token_identifier();

        /* Auxiliary */ 

        void handle_comment_single_line();
        
        void handle_comment();

        void handle_quotation_mark();

        void handle_single_quotation_mark();

    };

}

#endif