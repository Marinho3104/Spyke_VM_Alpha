#include "pre_compiler_exceptions.h"

#include "code_information.h"
#include "linked_list.h"
#include "token.h"

#include <iostream>
#include <string.h>

parser::Ordinary_Exception_Pre_Compiler::Ordinary_Exception_Pre_Compiler(const char* __info) : information(__info) {}

const char* parser::Ordinary_Exception_Pre_Compiler::what() const throw() { return information; }


parser::Unknow_Instruction_Pre_Compiler::Unknow_Instruction_Pre_Compiler(Code_Information* __code_information, Token* __token) {

    char* _inicial_line = 
            __code_information->get_line(__token->position_information.line);

        char* _error_sign = 
            __code_information->get_line_error_sign(__token->position_information.line, __token->position_information.column, __token->position_information.size);

        asprintf(
            &information,
            "\n\n" \
            "\tUnknow Instruction:\n\n" \
            "\t\t%s\n" \
            "\t\t%s\n",
            _inicial_line,
            _error_sign
        ); 

        free(_inicial_line); free(_error_sign);

}

const char* parser::Unknow_Instruction_Pre_Compiler::what() const throw() { return (const char*) information; }


parser::Few_Arguments_Pre_Compiler::Few_Arguments_Pre_Compiler(Code_Information* __code_information, int __instruction_id, int __line, int __column) {

    char* _code_line = 
        __code_information->get_line(__line);

    char* _error_sign = 
        __code_information->get_line_error_sign(
            __line, 
            __column, 
            strlen(
                __code_information->code_by_lines->operator[](
                    __line
                )
            ) - __column
        );

    asprintf(
        &information,
        "\n\n" \
        "\tFew Arguments for given Pre Compiler Instruction (id: %i):\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        __instruction_id, 
        _code_line,
        _error_sign
    ); 

    free(_code_line);
    free(_error_sign); 

}

const char* parser::Few_Arguments_Pre_Compiler::what() const throw() { return (const char*) information; }


parser::Expected_Identifier_Pre_Compiler::Expected_Identifier_Pre_Compiler(Code_Information* __code_information, int __line, int __column) {

    char* _code_line = 
        __code_information->get_line(__line);

    char* _error_sign = 
        __code_information->get_line_error_sign(
            __line, 
            __column, 
            1
        );

    asprintf(
        &information,
        "\n\n" \
        "\tExpected Token Identifier:\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        _code_line,
        _error_sign
    ); 

    free(_code_line);
    free(_error_sign);  

}

const char* parser::Expected_Identifier_Pre_Compiler::what() const throw() { return (const char*) information; }

