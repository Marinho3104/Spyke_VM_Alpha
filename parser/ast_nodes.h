#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

#include "parser_definitions.h"

namespace parser {

    struct Ast_Node {

        int node_type; 

        virtual ~Ast_Node(); Ast_Node(int);

    };

    struct Ast_Node_Name_Space : Ast_Node {

        Name_Space* name_space;
        
        utils::Linked_List <Ast_Node*>* declarations;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        static Ast_Node_Name_Space* generate(Ast*, Name_Space*);

        static Ast_Node_Name_Space* generate(Ast*);

    };

}

#endif