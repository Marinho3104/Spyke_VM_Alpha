#pragma once


#ifndef PARSER_PRE_COMPILER_EXCEPTIONS_H
#define PARSER_PRE_COMPILER_EXCEPTIONS_H

#include "parser_definitions.h"

#include <exception>
#include <iostream>

namespace parser {

    struct Ordinary_Exception_Pre_Compiler : public std::exception {

        const char* information;

        Ordinary_Exception_Pre_Compiler(const char* __info);

        const char* what() const throw();

    };

    struct Unknow_Instruction_Pre_Compiler : public std::exception {

        char* information;

        Unknow_Instruction_Pre_Compiler(Code_Information*, Token*);

        const char* what() const throw();

    };

    struct Few_Arguments_Pre_Compiler : public std::exception {

        char* information;

        Few_Arguments_Pre_Compiler(Code_Information*, int, int, int);

        const char* what() const throw();

    };

    struct Expected_Identifier_Pre_Compiler : public std::exception {

        char* information;

        Expected_Identifier_Pre_Compiler(Code_Information*, int, int);

        const char* what() const throw();

    };

}

#endif