#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser_definitions.h"

namespace parser {

    struct Ast {

        utils::Linked_List <Ast_Node*>* current_nodes;

        utils::Linked_List <Ast_Node*>* open_nodes;
        utils::Linked_List <Name_Space*>* name_space_chain;
        utils::Linked_List <Token*>* tokens_collection;
        Implicit_Values_Tracker* implicit_values;
        Ast_Node_Name_Space* global_name_space;
        Name_Space_Control* name_space_control;
        Code_Information* code_information;
        int tokens_position;

        ~Ast(); Ast(Code_Information*, utils::Linked_List <Token*>*);

        void print(const char*);

        Token* get_token(int);

        void generate_ast_nodes();

        int add_implicit_value(Token*);

        void add_to_chain(Name_Space*); void pop_from_chain();

        void join_with_current_path(utils::Linked_List <char*>*);

    };

}

#endif