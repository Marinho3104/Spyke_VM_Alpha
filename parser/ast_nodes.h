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

    struct Ast_Node_Code_Block : Ast_Node {

        Ast_Node_Code_Block* previous;
        Name_Space* name_space;

        utils::Linked_List <Ast_Node*>* code;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(Ast_Node_Code_Block*, Name_Space*);

        void set_code(Ast*);

        static Ast_Node_Code_Block* generate(Ast*);

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
        
        static utils::Linked_List <Ast_Node*>* generate_function_parameters(Ast*);
                
        static utils::Linked_List <Ast_Node*>* generate(Ast*);

        static void ignore(Ast*);

    };

    struct Ast_Node_Function_Declaration : Ast_Node {

        utils::Linked_List <Type_Information*>* parameters_type;
        Token* function_token_name;
        Ast_Node_Code_Block* body;
        Name_Space* name_space;
        bool is_static;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(Type_Information*, Token*, Name_Space*, bool);

        void set_this_variable(Ast*);

        void set_parameters(Ast*);

        static Ast_Node_Function_Declaration* generate(Ast*);

        static void ignore(Ast*);

    };

    struct Ast_Node_Expression : Ast_Node {

        utils::Linked_List <Ast_Node*>* values;
        utils::Linked_List <int>* token_ids;

        ~Ast_Node_Expression(); Ast_Node_Expression();

        static Ast_Node_Expression* generate(Ast*, int);

    };

}

#endif