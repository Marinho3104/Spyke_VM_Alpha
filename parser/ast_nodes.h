#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

#include "parser_definitions.h"

namespace parser {

    struct Ast_Node {

        int node_type;
        Type_Information* representive_type;

        virtual ~Ast_Node(); Ast_Node(int, Type_Information*);

    };

    struct Ast_Node_Name_Space : Ast_Node {

        Name_Space* name_space;
        
        utils::Linked_List <Ast_Node*>* declarations;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        void set_declarations(Ast*);

        void set_declarations_struct_body(Ast*);

        static Ast_Node_Name_Space* generate(Ast*, Name_Space*);

        static Ast_Node_Name_Space* generate(Ast*);

    };

    struct Ast_Node_Struct_Declaration : Ast_Node {

        Ast_Node_Name_Space* body;
        Token* struct_token_name;
        bool body_defined;

        ~Ast_Node_Struct_Declaration(); Ast_Node_Struct_Declaration(Token*, Name_Space*);

        static Ast_Node_Struct_Declaration* generate(Ast*);

    };

    struct Ast_Node_Variable_Declaration : Ast_Node {

        Token* variable_token_name;
        bool is_static;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(Type_Information*, Token*, bool);

        static Ast_Node_Variable_Declaration* generate(Ast*, Ast_Node_Struct_Declaration*, bool);
        
        static utils::Linked_List <Ast_Node*>* generate(Ast*);

    };

    struct Ast_Node_Function_Declaration : Ast_Node {};

}

#endif