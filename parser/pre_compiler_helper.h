#pragma once

#ifndef PARSER_PRE_COMPILER_HELPER_H
#define PARSER_PRE_COMPILER_HELPER_H

#include "parser_definitions.h"

namespace parser {

    struct Pre_Compiler_Define_Instruction {

        char* identifier;
        Token* replace;

        ~Pre_Compiler_Define_Instruction(); Pre_Compiler_Define_Instruction(char*, Token*);

        void handle(Token**);

    };

}

#endif