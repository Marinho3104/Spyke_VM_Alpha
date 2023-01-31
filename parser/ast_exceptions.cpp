#include "ast_exceptions.h"


#include "code_information.h"
#include "linked_list.h"
#include "token.h"

#include <iostream>


parser::Ordinary_Exception_Ast::Ordinary_Exception_Ast(const char* __info) : information(__info) {}

const char* parser::Ordinary_Exception_Ast::what() const throw() { return information; }


parser::Unexpected_Token_Ast::Unexpected_Token_Ast(Code_Information* __code_information, Token* __token) {

    char* _inicial_line = 
        __code_information->get_line(__token->position_information.line);

    char* _error_sign = 
        __code_information->get_line_error_sign(__token->position_information.line, __token->position_information.column, __token->position_information.size);

    asprintf(
        &information,
        "\n\n" \
        "\tUnexpected Token: (id: %i)\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        __token->id,
        _inicial_line,
        _error_sign
    ); 

    free(_inicial_line); free(_error_sign);

}

const char* parser::Unexpected_Token_Ast::what() const throw() { return (const char*) information; }


parser::Expected_Token_Ast::Expected_Token_Ast(Code_Information* __code_information, Token* __token, int __token_id) {

    char* _inicial_line = 
        __code_information->get_line(__token->position_information.line);

    char* _error_sign = 
        __code_information->get_line_error_sign(__token->position_information.line, __token->position_information.column, 1);

    asprintf(
        &information,
        "\n\n" \
        "\tExpected Token: (id: %i)\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        __token_id,
        _inicial_line,
        _error_sign
    ); 

    free(_inicial_line); free(_error_sign);

}

const char* parser::Expected_Token_Ast::what() const throw() { return (const char*) information; }


parser::Undefined_Name_Space_Path_Ast::Undefined_Name_Space_Path_Ast(
    Code_Information* __code_information, utils::Data_Linked_List <Token*>* __last_token, int __path_length) {

        char* _inicial_line = 
            __code_information->get_line(__last_token->object->position_information.line);

        int _final_column = __last_token->object->position_information.column + __last_token->object->position_information.size;

        while(__path_length) { __last_token = __last_token->previous; __path_length--; }

        int _size = _final_column - __last_token->object->position_information.column;

        char* _error_sign = 
            __code_information->get_line_error_sign(__last_token->object->position_information.line, __last_token->object->position_information.column, _size);

        asprintf(
            &information,
            "\n\n" \
            "\tUndefined Name Space Path:\n\n" \
            "\t\t%s\n" \
            "\t\t%s\n",
            _inicial_line,
            _error_sign
        ); 

        free(_inicial_line); free(_error_sign);


}

const char* parser::Undefined_Name_Space_Path_Ast::what() const throw() { return (const char*) information; }


parser::Undefined_Built_Ins_Name_Space_Path_Ast::Undefined_Built_Ins_Name_Space_Path_Ast(Code_Information* __code_information, Token* __token) {

    char* _inicial_line = 
        __code_information->get_line(__token->position_information.line);

    char* _error_sign = 
        __code_information->get_line_error_sign(__token->position_information.line, __token->position_information.column, __token->position_information.size);

    asprintf(
        &information,
        "\n\n" \
        "\tBuilt_Ins Name Space:\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        _inicial_line,
        _error_sign
    ); 

    free(_inicial_line); free(_error_sign);

}

const char* parser::Undefined_Built_Ins_Name_Space_Path_Ast::what() const throw() { return (const char*) information; }


parser::Undefined_Struct_Declaration_Ast::Undefined_Struct_Declaration_Ast(Code_Information* __code_information, Token* __token, int __inicial_position) {

    char* _inicial_line = 
        __code_information->get_line(__token->position_information.line);

    char* _error_sign = 
        __code_information->get_line_error_sign(
            __token->position_information.line, 
            __inicial_position, 
            __token->position_information.size + __token->position_information.column - __inicial_position
        );

    asprintf(
        &information,
        "\n\n" \
        "\tUndefined type:\n\n" \
        "\t\t%s\n" \
        "\t\t%s\n",
        _inicial_line,
        _error_sign
    ); 

    free(_inicial_line); free(_error_sign);


}

const char* parser::Undefined_Struct_Declaration_Ast::what() const throw() { return (const char*) information; }


parser::Redefinition_Struct_Declaration_Ast::Redefinition_Struct_Declaration_Ast(
    Code_Information* __code_information, Token* __previous_declaration, Token* __current_declaration) {

        char* _previous_struct_declaration_line = 
            __code_information->get_line(__previous_declaration->position_information.line);

        char* _previous_struct_declaration_error_sign = 
            __code_information->get_line_error_sign(
                __previous_declaration->position_information.line, 
                __previous_declaration->position_information.column, 
                __previous_declaration->position_information.size
            );

        char* _current_struct_declaration_line = 
            __code_information->get_line(__current_declaration->position_information.line);

        char* _current_struct_declaration_error_sign = 
            __code_information->get_line_error_sign(
                __current_declaration->position_information.line, 
                __current_declaration->position_information.column, 
                __current_declaration->position_information.size
            );


        asprintf(
            &information,
            "\n\n" \
            "\tRedefinition Struct Declaration:\n\n" \
            "\t\tPrevious declaration:\n\n" \
            "\t\t\t%s\n" \
            "\t\t\t%s\n\n" \
            "\t\tDeclaration:\n\n" \
            "\t\t\t%s\n" \
            "\t\t\t%s\n",
            _previous_struct_declaration_line,
            _previous_struct_declaration_error_sign,
            _current_struct_declaration_line,
            _current_struct_declaration_error_sign
        ); 

        free(_previous_struct_declaration_line); free(_previous_struct_declaration_error_sign); 

        free(_current_struct_declaration_line); free(_current_struct_declaration_error_sign);

}

const char* parser::Redefinition_Struct_Declaration_Ast::what() const throw() { return (const char*) information; }


parser::Undefined_Variable_Declaration_Ast::Undefined_Variable_Declaration_Ast(Code_Information* __code_information, Token* __token, int __incial_position) {

}

const char* parser::Undefined_Variable_Declaration_Ast::what() const throw() { return (const char*) information; }


parser::Redefinition_Variable_Declaration_Ast::Redefinition_Variable_Declaration_Ast(Code_Information* __code_information, Token* __previous_declaration, Token* __current_declaration) {

        char* _previous_variable_declaration_line = 
            __code_information->get_line(__previous_declaration->position_information.line);

        char* _previous_variable_declaration_error_sign = 
            __code_information->get_line_error_sign(
                __previous_declaration->position_information.line, 
                __previous_declaration->position_information.column, 
                __previous_declaration->position_information.size
            );

        char* _current_variable_declaration_line = 
            __code_information->get_line(__current_declaration->position_information.line);

        char* _current_variable_declaration_error_sign = 
            __code_information->get_line_error_sign(
                __current_declaration->position_information.line, 
                __current_declaration->position_information.column, 
                __current_declaration->position_information.size
            );


        asprintf(
            &information,
            "\n\n" \
            "\tRedefinition Variable Declaration:\n\n" \
            "\t\tPrevious declaration:\n\n" \
            "\t\t\t%s\n" \
            "\t\t\t%s\n\n" \
            "\t\tDeclaration:\n\n" \
            "\t\t\t%s\n" \
            "\t\t\t%s\n",
            _previous_variable_declaration_line,
            _previous_variable_declaration_error_sign,
            _current_variable_declaration_line,
            _current_variable_declaration_error_sign
        ); 

        free(_previous_variable_declaration_line); free(_previous_variable_declaration_error_sign); 

        free(_current_variable_declaration_line); free(_current_variable_declaration_error_sign);


}

const char* parser::Redefinition_Variable_Declaration_Ast::what() const throw() { return (const char*) information; }
