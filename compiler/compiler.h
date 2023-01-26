#pragma once

#ifndef VIRTUAL_MACHINE_COMPILER_H
#define VIRTUAL_MACHINE_COMPILER_H

#include "compiler_definitions.h"

#define COMPILER_MODE_FULL_COMPILATION 0

namespace compiler {

    struct Compiler {

        char* code;

        /* Parser Proccesses */
        parser::Code_Information* code_information;
        parser::Pre_Compiler* pre_compiler;
        parser::Tokenizer* tokenizer;

        ~Compiler(); Compiler(const char*, int);

        void full_compilation();

    };


}

#endif