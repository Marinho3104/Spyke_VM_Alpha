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
    struct Ast_Node_Variable;
    struct Ast_Node_Accessing;
    struct Ast_Node_Code_Block;
    struct Ast_Node_Name_Space;
    struct Ast_Node_Expression;
    struct Ast_Node_Parenthesis;
    struct Ast_Node_Function_Call;
    struct Ast_Node_Implicit_Value;
    struct Ast_Node_Pointer_Operation;
    struct Ast_Node_Struct_Declaration;
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Constructor_Call;

    // Ast Helper
    struct Type_Information;

    // Convertor
    struct Convertor;


}

/* Byte Code */
namespace byte_code {

    // Byte Code
    struct Byte_Code;

    // Byte Code Block
    struct Byte_Code_Block;

    // Compiled Byte Code
    struct Compiled_Byte_Code;

}

#endif