#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

/* utils */
namespace utils {

    template <typename> struct Linked_List;
    template <typename> struct Data_Linked_List;

}

/* Parser */
namespace parser {

    // Code Informaiton
    struct Code_Information; 

    // Tokenizer
    struct Tokenizer;

    struct Token;

    // Pre Compiler
    struct Pre_Compiler;

    struct Pre_Compiler_Define_Instruction;

    // Ast
    struct Ast;

    struct Name_Space_Control;
    struct Name_Space;

    struct Ast_Node;
    struct Ast_Node_Code_Block;
    struct Ast_Node_Name_Space;
    struct Ast_Node_Expression;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;

    // Ast Helper
    struct Type_Information;


}

#endif