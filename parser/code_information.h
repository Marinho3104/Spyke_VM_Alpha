#pragma once 

#ifndef PARSER_CODE_INFORMATION_H
#define PARSER_CODE_INFORMATION_H

#include "parser_definitions.h"

namespace parser {

    struct Code_Information {

        utils::Linked_List <char*>* code_by_lines;
        char* code;

        ~Code_Information(); Code_Information(char*);

        void set_code_by_lines();

        
        utils::Linked_List <char*>* get_line_preview(int);
        
        char* get_line(int);
        
        char* get_line_padding(int);

        char* get_line_error_sign(int, int, int);

    };


}

#endif