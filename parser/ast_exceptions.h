#pragma once

#ifndef PARSER_AST_NODES_EXCEPTIONS_H
#define PARSER_AST_NODES_EXCEPTIONS_H

#include "parser_definitions.h"

#include <exception>
#include <iostream>

namespace parser {

    struct Ordinary_Exception_Ast : public std::exception {

        const char* information;

        Ordinary_Exception_Ast(const char* __info);

        const char* what() const throw();

    };

    struct Unexpected_Token_Ast : public std::exception {

        char* information;

        Unexpected_Token_Ast(Code_Information*, Token*);

        const char* what() const throw();

    };

    struct Expected_Token_Ast : public std::exception {

        char* information;

        Expected_Token_Ast(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Undefined_Name_Space_Path_Ast : public std::exception {

        char* information;

        Undefined_Name_Space_Path_Ast(Code_Information*, utils::Data_Linked_List <Token*>*, int);

        const char* what() const throw();

    };

    struct Undefined_Built_Ins_Name_Space_Path_Ast : public std::exception {

        char* information;

        Undefined_Built_Ins_Name_Space_Path_Ast(Code_Information*, Token*);

        const char* what() const throw();

    };

    struct Undefined_Struct_Declaration_Ast : public std::exception {

        char* information;

        Undefined_Struct_Declaration_Ast(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Redefinition_Struct_Declaration_Ast : public std::exception {

        char* information;

        Redefinition_Struct_Declaration_Ast(Code_Information*, Token*, Token*);

        const char* what() const throw();        

    };

    struct Undefined_Variable_Declaration_Ast : public std::exception {

        char* information;

        Undefined_Variable_Declaration_Ast(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Redefinition_Variable_Declaration_Ast : public std::exception {

        char* information;

        Redefinition_Variable_Declaration_Ast(Code_Information*, Token*, Token*);

        const char* what() const throw();        

    };

}

#endif