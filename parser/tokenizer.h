#pragma once

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

#include "parser_definitions.h"
#include <stddef.h>

namespace parser {

    struct Tokenizer {

        utils::Linked_List <Token*>* tokens_collection;

        char* code, *code_to_free, *collumn_inicial_address;

        size_t current_line;


        ~Tokenizer(); Tokenizer(char*);

        void set_tokens();

        void print_tokens();

        /* Functions */

        void handle_new_token();

        bool handle_token_symbol();

        /* Auxiliary */ 

        void handle_comment_single_line();
        
        void handle_comment();

        void handle_quotation_mark();

        void handle_single_quotation_mark();

    };

}

#endif