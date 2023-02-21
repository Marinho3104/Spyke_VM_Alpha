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

        ~Unexpected_Token_Ast();

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

        ~Undefined_Struct_Declaration_Ast();

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

    struct Undefined_Function_Declaration_Ast : public std::exception {

        char* information;

        ~Undefined_Function_Declaration_Ast();

        Undefined_Function_Declaration_Ast(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Redefinition_Function_Declaration_Ast : public std::exception {

        char* information;

        Redefinition_Function_Declaration_Ast(Code_Information*, Token*, Token*);

        const char* what() const throw();        

    };

    struct Pointer_Operations_Below_0_Ast : public std::exception {

        char* information;

        Pointer_Operations_Below_0_Ast(Code_Information*, utils::Linked_List <Token*>*);

        const char* what() const throw();      

    };

    struct Unmatched_Accessing_Operation_Ast : public std::exception {

        char* information;

        Unmatched_Accessing_Operation_Ast(Code_Information*, Token*);

        const char* what() const throw();

    };

    struct Struct_Body_Not_Defined_Ast : public std::exception {

        char* information;

        ~Struct_Body_Not_Defined_Ast();

        Struct_Body_Not_Defined_Ast(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Undefined_Constructor_Declaration_Ast : public std::exception {

        char* information;

        Undefined_Constructor_Declaration_Ast(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Default_Constructor_Undefined_Declaration_Ast : public std::exception {

        char* information;

        Default_Constructor_Undefined_Declaration_Ast(Code_Information*, Token*);

        const char* what() const throw();

    };

    struct Constructor_With_Given_Parameters_Undefined_Declaration_Ast : public std::exception {

        char* information;

        Constructor_With_Given_Parameters_Undefined_Declaration_Ast(Code_Information*, Token*);

        const char* what() const throw();

    };

    struct Function_Return_And_Given_Return_Not_The_Same : public std::exception {

        char* information;

        Function_Return_And_Given_Return_Not_The_Same(Code_Information*, Token*, int);

        const char* what() const throw();

    };

    struct Control_Struct_Key_Word_Out_Of_Control_Struct : public std::exception {

        char* information;

        Control_Struct_Key_Word_Out_Of_Control_Struct(Code_Information*, Token*);

        const char* what() const throw();

    };

}

#endif