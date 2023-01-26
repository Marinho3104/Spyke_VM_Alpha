#pragma once

#ifndef PARSER_TOKENIZER_EXCEPTIONS_H
#define PARSER_TOKENIZER_EXCEPTIONS_H

#include "parser_definitions.h"

#include <exception>
#include <iostream>

namespace parser {
    
    struct Ordinary_Exception_Tokenizer : public std::exception {

        const char* information;

        Ordinary_Exception_Tokenizer(const char* __info);

        const char* what() const throw();

    };

    struct Expected_Token_Exception_Tokenizer : public std::exception {

        char* information;

        Expected_Token_Exception_Tokenizer(Code_Information*, int, int, int, int);

        const char* what() const throw();

    };

    struct Implicit_Value_Bad_Structure_Exception_Tokenizer : public std::exception {

        char* information;

        Implicit_Value_Bad_Structure_Exception_Tokenizer(Tokenizer*);

        const char* what() const throw();

    };

}

#endif