#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

/* utils */
namespace utils {

    template <typename> struct Linked_List;

}

/* Parser */
namespace parser {

    // Code Informaiton
    struct Code_Information; 

    // Tokenizer
    struct Tokenizer;

    // Pre Compiler
    struct Pre_Compiler;

    struct Pre_Compiler_Define_Instruction;

    // Ast
    struct Ast;

    struct Name_Space_Control;
    struct Name_Space;

    struct Ast_Node;
    struct Ast_Node_Name_Space;


    struct Token;

}

#endif