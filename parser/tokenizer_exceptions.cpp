#include "tokenizer_exceptions.h"

#include "code_information.h"
#include "linked_list.h"
#include "tokenizer.h"
#include "token.h"

#include <iostream>


parser::Ordinary_Exception_Tokenizer::Ordinary_Exception_Tokenizer(const char* __info) : information(__info) {}

const char* parser::Ordinary_Exception_Tokenizer::what() const throw() { return information; }


parser::Expected_Token_Exception_Tokenizer::Expected_Token_Exception_Tokenizer(
    Code_Information* __code_information, int __token_id, int __token_length, int __inicial_current_line, int __inicial_current_position) {

        char* _inicial_line = 
            __code_information->get_line(__inicial_current_line);

        char* _error_sign = 
            __code_information->get_line_error_sign(__inicial_current_line, __inicial_current_position, __token_length);

        asprintf(
            &information,
            "\n\n" \
            "\tExpected Token (id: %i):\n\n" \
            "\t\t%s\n" \
            "\t\t%s\n",
            __token_id, 
            _inicial_line,
            _error_sign
        ); 

        free(_inicial_line); free(_error_sign);

}

const char* parser::Expected_Token_Exception_Tokenizer::what() const throw() { return (const char*) information; }


parser::Implicit_Value_Bad_Structure_Exception_Tokenizer::Implicit_Value_Bad_Structure_Exception_Tokenizer(Tokenizer* __tokenizer) {

    char* _inicial_line = 
        __tokenizer->code_information->get_line(
            __tokenizer->tokens_collection->operator[](
                __tokenizer->tokens_collection->count
            )->position_information.line
        );

    char* _error_sign = 
        __tokenizer->code_information->get_line_error_sign(
            __tokenizer->tokens_collection->operator[](
                __tokenizer->tokens_collection->count
            )->position_information.line,
            __tokenizer->tokens_collection->operator[](
                __tokenizer->tokens_collection->count
            )->position_information.column,
            __tokenizer->tokens_collection->operator[](
                __tokenizer->tokens_collection->count
            )->position_information.size
        );


    asprintf(
        &information,
        "\n\n" \
        "\tExpected Token Identifier (id: %i):\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        __tokenizer->tokens_collection->operator[](
                __tokenizer->tokens_collection->count
            )->id, 
        _inicial_line,
        _error_sign
    ); 

    free(_inicial_line); free(_error_sign);

}

const char* parser::Implicit_Value_Bad_Structure_Exception_Tokenizer::what() const throw() { return information; }
