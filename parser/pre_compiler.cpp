#include "pre_compiler.h"

#include "tokenizer.h"
#include "code_information.h"

#include "pre_compiler_exceptions.h"
#include "pre_compiler_warnings.h"
#include "pre_compiler_helper.h"
#include "token_definitions.h"
#include "linked_list.h"
#include "compiler.h"
#include "common.h"
#include "token.h"

#include <iostream>
#include <string.h>

parser::Pre_Compiler::~Pre_Compiler() { delete active_define_instructions; }

parser::Pre_Compiler::Pre_Compiler(Code_Information* __code_information, utils::Linked_List <Token*>* __tokens_collection) 
    : tokens_collection(__tokens_collection), tokens_position(0), code_information(__code_information)
        { active_define_instructions = new utils::Linked_List <Pre_Compiler_Define_Instruction*>(); execute(0); }

void parser::Pre_Compiler::execute(Token* __token) {

    while (tokens_position < tokens_collection->count) {

        if (tokens_collection->operator[](tokens_position)->id == IDENTIFIER && !handle_identifier()) continue;

        if (tokens_collection->operator[](tokens_position)->id != PRE_COMPILER) 
            { tokens_position++; continue; }

        if (__token && check_end_block_pre_compiler_instruction()) break;

        execute_pre_compiler_instruction();

    }

    if (tokens_position == tokens_collection->count && __token)

        throw Expected_Token_Pre_Compiler(code_information, PRE_COMPILER_END_IF, __token->position_information.line, __token->position_information.column);


}

void parser::Pre_Compiler::skip_block(Token* __token) {

    while (tokens_position < tokens_collection->count) {
    
        if (tokens_collection->operator[](tokens_position)->id == PRE_COMPILER && check_end_block_pre_compiler_instruction()) break;

        else delete tokens_collection->remove(tokens_position);
    
    }

    if (tokens_position == tokens_collection->count) 
    
        throw Expected_Token_Pre_Compiler(code_information, PRE_COMPILER_END_IF, __token->position_information.line, __token->position_information.column);

}


void parser::Pre_Compiler::execute_pre_compiler_instruction() {

    switch (tokens_collection->operator[](tokens_position + 1)->id)
    {
    case PRE_COMPILER_DEFINE: execute_pre_compiler_define_instruction(); break;
    case PRE_COMPILER_INCLUDE: execute_pre_compiler_include_instruction(); break;
    case PRE_COMPILER_IF_DEF: execute_pre_compiler_if_define_instruction(); break;
    case PRE_COMPILER_IF_N_DEF: execute_pre_compiler_if_not_define_instruction(); break;
    case PRE_COMPILER_ELSE: case PRE_COMPILER_END_IF: throw Unexpected_Instruction_Pre_Compiler(code_information, tokens_collection->operator[](tokens_position + 1)); break;
    default: throw Unknow_Instruction_Pre_Compiler(code_information, tokens_collection->operator[](tokens_position + 1)); break;
    }

}

bool parser::Pre_Compiler::check_end_block_pre_compiler_instruction() {

    bool _result = 0;

    switch (tokens_collection->operator[](tokens_position + 1)->id)
    {
    case PRE_COMPILER_ELSE: case PRE_COMPILER_END_IF: _result = 1; break;
    default: break;
    }

    return _result;

}


void parser::Pre_Compiler::execute_pre_compiler_define_instruction() {

    #define DEFINE_INSTRUCTION_MIN_ARGS 3
    #define DEFINE_INSTRUCTION_MAX_NEEDED_ARGS 4

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;

    utils::Linked_List <Token*>* _define_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->getDataLinkedList(tokens_position) && 
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
        throw Expected_Token_Pre_Compiler(code_information, IDENTIFIER, _line, _define_instruction_tokens_collection->operator[](DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

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
            _define_instruction_tokens_collection->count < 4 ? 
                0 : _define_instruction_tokens_collection->operator[](3)
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

bool parser::Pre_Compiler::handle_identifier() {

    for (int _ = 0; _ < active_define_instructions->count; _++)

        if (
            !strcmp(
                active_define_instructions->operator[](_)->identifier,
                tokens_collection->operator[](tokens_position)->identifier
            )
        ) 
            if (active_define_instructions->operator[](_)->replace)

                active_define_instructions->operator[](_)->handle(
                    &tokens_collection->getDataLinkedList(tokens_position)->object
                );

            else { delete tokens_collection->remove(tokens_position); return 0; }

    return 1;

}

void parser::Pre_Compiler::handle_identifier(Token** __token) {


    for (int _ = 0; _ < active_define_instructions->count; _++)

        if (
            !strcmp(
                active_define_instructions->operator[](_)->identifier,
                (*__token)->identifier
            )
        ) 
            active_define_instructions->operator[](_)->handle(
                __token
            );

}


void parser::Pre_Compiler::execute_pre_compiler_include_instruction() {

    #define INCLUDE_INSTRUCTION_MIN_ARGS 3
    #define INCLUDE_INSTRUCTION_MAX_NEEDED_ARGS 3

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;

    utils::Linked_List <Token*>* _include_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->getDataLinkedList(tokens_position) && 
        tokens_collection->operator[](tokens_position)->position_information.line == _line
    ) {

        utils::Data_Linked_List <Token*>* _token_data_ll = 
            tokens_collection->remove(tokens_position);
        _token_data_ll->destroy_content = 0;
    
        _include_instruction_tokens_collection->add(
            _token_data_ll->object
        );

        delete _token_data_ll;

    }

    if (_include_instruction_tokens_collection->count < INCLUDE_INSTRUCTION_MIN_ARGS) 
        throw Few_Arguments_Pre_Compiler(code_information, PRE_COMPILER_INCLUDE, _line, _include_instruction_tokens_collection->first->object->position_information.column);

    if (_include_instruction_tokens_collection->count > INCLUDE_INSTRUCTION_MAX_NEEDED_ARGS)
        More_Arguments_Than_Needed_Warning::launch(code_information, PRE_COMPILER_INCLUDE, _line, _include_instruction_tokens_collection->operator[](INCLUDE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    if (_include_instruction_tokens_collection->operator[](2)->id == IDENTIFIER)
        handle_identifier(&_include_instruction_tokens_collection->getDataLinkedList(2)->object);

    if (_include_instruction_tokens_collection->operator[](2)->id != IMPLICIT_VALUE_STRING)
        throw Expected_Token_Pre_Compiler(code_information, IMPLICIT_VALUE_STRING, _line, _include_instruction_tokens_collection->operator[](INCLUDE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);
    
    if (!utils::file_exists(_include_instruction_tokens_collection->operator[](2)->identifier))
        throw Unknow_Include_Path_Pre_Compiler(code_information, PRE_COMPILER_INCLUDE, _line, _include_instruction_tokens_collection->first->object->position_information.column);

    compiler::Compiler* _compiler = new compiler::Compiler(
        (const char*) _include_instruction_tokens_collection->operator[](2)->identifier, 
        COMPILER_MODE_TOKENIZER_COMPILATION
    );

    delete code_information->code_by_lines->remove(_line);

    for (int _ = 0; _ < _compiler->tokenizer->tokens_collection->count; _++)

        _compiler->tokenizer->tokens_collection->operator[](_)->position_information.line += _line;

    int _jump = _compiler->code_information->code_by_lines->count - 1;

    for (int _ = tokens_position; _ < tokens_collection->count; _++)

        tokens_collection->operator[](_)->position_information.line += _jump;


    code_information->code_by_lines->join(
        _compiler->code_information->code_by_lines,
        _line
    );

    tokens_collection->join(
        _compiler->tokenizer->tokens_collection,
        tokens_position
    );

    _compiler->tokenizer->tokens_collection->destroy_content = 0;
    _compiler->code_information->code_by_lines->destroy_content = 0;

    delete _include_instruction_tokens_collection;
    delete _compiler;

}


void parser::Pre_Compiler::execute_pre_compiler_if_define_instruction() {

    #define IF_DEFINE_INSTRUCTION_MIN_ARGS 3
    #define IF_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS 3

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;

    utils::Linked_List <Token*>* _if_define_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->getDataLinkedList(tokens_position) && 
        tokens_collection->operator[](tokens_position)->position_information.line == _line
    ) {

        utils::Data_Linked_List <Token*>* _token_data_ll = 
            tokens_collection->remove(tokens_position);
        _token_data_ll->destroy_content = 0;
    
        _if_define_instruction_tokens_collection->add(
            _token_data_ll->object
        );

        delete _token_data_ll;

    }

    if (_if_define_instruction_tokens_collection->count < IF_DEFINE_INSTRUCTION_MIN_ARGS) 
        throw Few_Arguments_Pre_Compiler(code_information, PRE_COMPILER_IF_DEF, _line, _if_define_instruction_tokens_collection->first->object->position_information.column);

    if (_if_define_instruction_tokens_collection->count > IF_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)
        More_Arguments_Than_Needed_Warning::launch(code_information, PRE_COMPILER_IF_DEF, _line, _if_define_instruction_tokens_collection->operator[](IF_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    if (_if_define_instruction_tokens_collection->operator[](2)->id != IDENTIFIER) 
        throw Expected_Token_Pre_Compiler(code_information, IDENTIFIER, _line, _if_define_instruction_tokens_collection->operator[](IF_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    
    if (
        get_previous_token_same_identifier(
            _if_define_instruction_tokens_collection->operator[](2)->identifier
        )
    ) {

        execute(_if_define_instruction_tokens_collection->operator[](1));

        handle_pre_compiler_else_or_end_if(1);

    }

    else {

        skip_block(_if_define_instruction_tokens_collection->operator[](1));

        handle_pre_compiler_else_or_end_if(0);
        
    }

    delete _if_define_instruction_tokens_collection;

}


void parser::Pre_Compiler::execute_pre_compiler_if_not_define_instruction() {

    #define IF_NOT_DEFINE_INSTRUCTION_MIN_ARGS 3
    #define IF_NOT_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS 3

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;

    utils::Linked_List <Token*>* _if_not_define_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->getDataLinkedList(tokens_position) && 
        tokens_collection->operator[](tokens_position)->position_information.line == _line
    ) {

        utils::Data_Linked_List <Token*>* _token_data_ll = 
            tokens_collection->remove(tokens_position);
        _token_data_ll->destroy_content = 0;
    
        _if_not_define_instruction_tokens_collection->add(
            _token_data_ll->object
        );

        delete _token_data_ll;

    }

    if (_if_not_define_instruction_tokens_collection->count < IF_NOT_DEFINE_INSTRUCTION_MIN_ARGS) 
        throw Few_Arguments_Pre_Compiler(code_information, PRE_COMPILER_IF_N_DEF, _line, _if_not_define_instruction_tokens_collection->first->object->position_information.column);

    if (_if_not_define_instruction_tokens_collection->count > IF_NOT_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)
        More_Arguments_Than_Needed_Warning::launch(code_information, PRE_COMPILER_IF_N_DEF, _line, _if_not_define_instruction_tokens_collection->operator[](IF_NOT_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    if (_if_not_define_instruction_tokens_collection->operator[](2)->id != IDENTIFIER) 
        throw Expected_Token_Pre_Compiler(code_information, IDENTIFIER, _line, _if_not_define_instruction_tokens_collection->operator[](IF_NOT_DEFINE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    
    if (
        get_previous_token_same_identifier(
            _if_not_define_instruction_tokens_collection->operator[](2)->identifier
        )
    ) {

        skip_block(_if_not_define_instruction_tokens_collection->operator[](1));

        handle_pre_compiler_else_or_end_if(0);

    }

    else {

        execute(_if_not_define_instruction_tokens_collection->operator[](1));

        handle_pre_compiler_else_or_end_if(1);
        
    }

    delete _if_not_define_instruction_tokens_collection;

}


void parser::Pre_Compiler::handle_pre_compiler_else_or_end_if(bool __ignore_block) {

    switch (tokens_collection->operator[](tokens_position + 1)->id)
    {
    case PRE_COMPILER_ELSE: execute_pre_compiler_else_instruction(__ignore_block); break;
    case PRE_COMPILER_END_IF: execute_pre_compiler_end_if_instruction(); break;
    default: break;
    }

}

void parser::Pre_Compiler::execute_pre_compiler_else_instruction(bool __ignore_block) {

    #define ELSE_INSTRUCTION_MIN_ARGS 2
    #define ELSE_INSTRUCTION_MAX_NEEDED_ARGS 2

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;

    utils::Linked_List <Token*>* _else_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->getDataLinkedList(tokens_position) && 
        tokens_collection->operator[](tokens_position)->position_information.line == _line
    ) {

        utils::Data_Linked_List <Token*>* _token_data_ll = 
            tokens_collection->remove(tokens_position);
        _token_data_ll->destroy_content = 0;
    
        _else_instruction_tokens_collection->add(
            _token_data_ll->object
        );

        delete _token_data_ll;

    }

    if (_else_instruction_tokens_collection->count > ELSE_INSTRUCTION_MAX_NEEDED_ARGS)
        More_Arguments_Than_Needed_Warning::launch(code_information, PRE_COMPILER_ELSE, _line, _else_instruction_tokens_collection->operator[](ELSE_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    if (__ignore_block) skip_block(_else_instruction_tokens_collection->operator[](1));

    else execute(_else_instruction_tokens_collection->operator[](1)); 

    delete _else_instruction_tokens_collection;

    handle_pre_compiler_else_or_end_if(1);

}

void parser::Pre_Compiler::execute_pre_compiler_end_if_instruction() {

    #define END_IF_INSTRUCTION_MIN_ARGS 2
    #define END_IF_INSTRUCTION_MAX_NEEDED_ARGS 2

    int _line = tokens_collection->operator[](tokens_position)->position_information.line;

    utils::Linked_List <Token*>* _end_if_instruction_tokens_collection = 
        new utils::Linked_List <Token*>();

    while(
        tokens_collection->getDataLinkedList(tokens_position) && 
        tokens_collection->operator[](tokens_position)->position_information.line == _line
    ) {

        utils::Data_Linked_List <Token*>* _token_data_ll = 
            tokens_collection->remove(tokens_position);
        _token_data_ll->destroy_content = 0;
    
        _end_if_instruction_tokens_collection->add(
            _token_data_ll->object
        );

        delete _token_data_ll;

    }

    if (_end_if_instruction_tokens_collection->count > END_IF_INSTRUCTION_MAX_NEEDED_ARGS)
        More_Arguments_Than_Needed_Warning::launch(code_information, PRE_COMPILER_END_IF, _line, _end_if_instruction_tokens_collection->operator[](END_IF_INSTRUCTION_MAX_NEEDED_ARGS)->position_information.column);

    delete _end_if_instruction_tokens_collection;

}






