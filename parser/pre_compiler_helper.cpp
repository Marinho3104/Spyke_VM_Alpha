#include "pre_compiler_helper.h"

#include "common.h"
#include "token.h"

#include <iostream>


parser::Pre_Compiler_Define_Instruction::~Pre_Compiler_Define_Instruction() { free(identifier); replace->~Token(); free(replace); }

parser::Pre_Compiler_Define_Instruction::Pre_Compiler_Define_Instruction(char* __identifier, Token* __replace) {

    identifier = utils::get_string_copy(__identifier);

    replace = __replace->get_copy();

}

void parser::Pre_Compiler_Define_Instruction::handle(Token** __token) {

    (*__token)->~Token(); free(*__token);

    *__token = replace->get_copy();

}



