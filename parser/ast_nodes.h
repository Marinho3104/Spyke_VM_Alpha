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

        bool is_pointer_struct_type();

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
        utils::Linked_List <Token*>* token_ids;

        ~Ast_Node_Expression(); Ast_Node_Expression();

        void set(Ast*);

        void set_representive_type(Ast*);

        static Ast_Node_Expression* generate(Ast*);

        static int get_operation_priority(int);

        static bool is_function_operator(int);

        static Ast_Node* get_value(Ast*);

    };

    struct Ast_Node_Variable : Ast_Node {

        Ast_Node_Variable_Declaration* variable_declaration; 
        Token* variable_token_name;

        ~Ast_Node_Variable(); Ast_Node_Variable(Ast_Node_Variable_Declaration*, Token*);

        static Ast_Node_Variable* generate(Ast*);

    };

    struct Ast_Node_Function_Call : Ast_Node {

        utils::Linked_List <Ast_Node_Expression*>* parameters;
        Ast_Node_Function_Declaration* function_declaration;
        Token* function_token_name;

        ~Ast_Node_Function_Call(); Ast_Node_Function_Call(Token*);

        void set_parameters(Ast*);

        void set_function_declaration(Ast*, Name_Space*, int);

        static Ast_Node_Function_Call* generate(Ast*);

    };

    struct Ast_Node_Implicit_Value : Ast_Node {

        int implicit_value_position;

        ~Ast_Node_Implicit_Value(); Ast_Node_Implicit_Value(Type_Information*, int);

        static Ast_Node_Implicit_Value* generate(Ast*);

    };

    struct Ast_Node_Pointer_Operation : Ast_Node {

        utils::Linked_List <Token*>* pointer_operations;
        int pointer_level;
        Ast_Node* value;

        ~Ast_Node_Pointer_Operation(); Ast_Node_Pointer_Operation(utils::Linked_List <Token*>*, int);

        void set_value(Ast*);

        void set_representive_value(Ast*);

        static Ast_Node_Pointer_Operation* generate(Ast*);

    };

    struct Ast_Node_Parenthesis : Ast_Node {

        Ast_Node_Expression* expression;

        ~Ast_Node_Parenthesis(); Ast_Node_Parenthesis();

        void set_expression(Ast*);

        void set_representive_type();

        static Ast_Node_Parenthesis* generate(Ast*);

    };

}

#endif