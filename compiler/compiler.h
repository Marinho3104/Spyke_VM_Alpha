#pragma once

#ifndef VIRTUAL_MACHINE_COMPILER_H
#define VIRTUAL_MACHINE_COMPILER_H

#include "compiler_definitions.h"

#define COMPILER_MODE_FULL_COMPILATION 0
#define COMPILER_MODE_TOKENIZER_COMPILATION 1

namespace compiler {

    struct Compiler {

        char* code;

        /* Parser Proccesses */
        parser::Code_Information* code_information;
        parser::Pre_Compiler* pre_compiler;
        parser::Tokenizer* tokenizer;
        parser::Ast* ast;

        ~Compiler(); Compiler(const char*, int);

        void full_compilation();

        void tokenizer_compilation();

    };


}

#endif