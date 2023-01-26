#include "pre_compiler.h"

#include "pre_compiler_exceptions.h"
#include "pre_compiler_warnings.h"
#include "pre_compiler_helper.h"
#include "token_definitions.h"
#include "linked_list.h"
#include "token.h"

#include <iostream>
#include <string.h>

parser::Pre_Compiler::~Pre_Compiler() { delete active_define_instructions; }

parser::Pre_Compiler::Pre_Compiler(Code_Information* __code_information, utils::Linked_List <Token*>* __tokens_collection) 
    : tokens_collection(__tokens_collection), tokens_position(0), code_information(__code_information)
        { active_define_instructions = new utils::Linked_List <Pre_Compiler_Define_Instruction*>(); execute(); }

void parser::Pre_Compiler::execute() {

    while (tokens_position < tokens_collection->count) {

        if (tokens_collection->operator[](tokens_position)->id == IDENTIFIER) 
            handle_identifier();


        if (tokens_collection->operator[](tokens_position)->id != PRE_COMPILER) 
            { tokens_position++; continue; }

        execute_pre_compiler_instruction();

    }

}


void parser::Pre_Compiler::execute_pre_compiler_instruction() {

    switch (tokens_collection->operator[](tokens_position + 1)->id)
    {
    case PRE_COMPILER_DEFINE: execute_pre_compiler_define_instruction(); break;
    default: throw Unknow_Instruction_Pre_Compiler(code_information, tokens_collection->operator[](tokens_position + 1)); break;
    }

}


void parser::Pre_Compiler::execute_pre_compiler_define_instruction() {

    #define DEFINE_INSTRUCTION_MIN_ARGS 3
    #define DEFINE_INSTRUCTION_MAX_NEEDED_ARGS 4

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;
    // int _column_position = tokens_collection->operator[](tokens_position)->position_information.column;

    utils::Linked_List <Token*>* _define_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->operator[](tokens_position)->position_information.line == _line
    ) {
    
        utils::Data_Linked_List <Token*>* _token_data_ll = 
            tokens_collection->remove(tokens_position);
        _token_data_ll->destroy_content = 0;
    
        _define_instruction_tokens_collection->add(
            _token_data_ll->object
        );

        delete _token_data_ll;

    }

    if (_define_instruction_tokens_collection->count < DEFINE_INSTRUCTION_MIN_ARGS) 
        throw Few_Arguments_Pre_Compiler(code_information, PRE_COMPILER_DEFINE, _line, _define_instruction_tokens_collection->first->object->position_information.column);

    if (_define_instruction_tokens_collection->count > DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)

        More_Arguments_Than_Needed_Warning::launch(code_information, PRE_COMPILER_DEFINE, _line, _define_instruction_tokens_collection->operator[](DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    if (_define_instruction_tokens_collection->operator[](2)->id != IDENTIFIER) 
        throw Ordinary_Exception_Pre_Compiler("Expected token identifier");

    Pre_Compiler_Define_Instruction* _define_instruction =
        get_previous_token_same_identifier(
            _define_instruction_tokens_collection->operator[](2)->identifier
        );
    
    if (_define_instruction) {

        _define_instruction->replace->~Token(); free(_define_instruction->replace);

        _define_instruction->replace = _define_instruction_tokens_collection->operator[](3)->get_copy();

    }

    else {

        Pre_Compiler_Define_Instruction* _define_instruction = (Pre_Compiler_Define_Instruction*) malloc(sizeof(Pre_Compiler_Define_Instruction));

        new (_define_instruction) Pre_Compiler_Define_Instruction(
            _define_instruction_tokens_collection->operator[](2)->identifier,
            _define_instruction_tokens_collection->operator[](3)
        );

        active_define_instructions->add(
            _define_instruction
        );

    }

    delete _define_instruction_tokens_collection;

}

parser::Pre_Compiler_Define_Instruction* parser::Pre_Compiler::get_previous_token_same_identifier(char* __identifier) {

    for (int _ = 0; _ < active_define_instructions->count; _++)

        if (
            !strcmp(
                active_define_instructions->operator[](_)->identifier,
                __identifier
            )
        ) return active_define_instructions->operator[](_);

    return 0;

}

void parser::Pre_Compiler::handle_identifier() {


    for (int _ = 0; _ < active_define_instructions->count; _++) {

        if (
            !strcmp(
                active_define_instructions->operator[](_)->identifier,
                tokens_collection->operator[](tokens_position)->identifier
            )
        ) 
            active_define_instructions->operator[](_)->handle(
                &tokens_collection->getDataLinkedList(tokens_position)->object
            );

    }

}



