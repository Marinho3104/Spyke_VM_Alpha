#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser_definitions.h"

namespace parser {

    struct Ast {

        Ast_Node_Name_Space* global_name_space;

        Name_Space_Control* name_space_control;

        utils::Linked_List <Token*>* tokens_collection;
        utils::Linked_List <Token*>* implicit_values;

        utils::Linked_List <Ast_Node*>* open_nodes;

        utils::Linked_List <Name_Space*>* name_space_chain;

        int tokens_position;

        ~Ast(); Ast(Code_Information*, utils::Linked_List <Token*>*);

        void print(const char*);

        void generate_ast_nodes();

        void add_to_chain(Name_Space*); void pop_from_chain();

    };

}

#endif