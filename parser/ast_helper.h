#pragma once 

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

#include "parser_definitions.h"

namespace parser {

    struct Declaration_Tracker {

        utils::Linked_List <parser::Ast_Node_Struct_Declaration*>* struct_declarations;
        utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* variable_declarations;
        utils::Linked_List <parser::Ast_Node_Function_Declaration*>* function_declarations;

        ~Declaration_Tracker(); Declaration_Tracker();

        Ast_Node_Struct_Declaration* get_struct_declaration(char*);

        Ast_Node_Variable_Declaration* get_variable_declaration(char*);

    };

    struct Name_Space {

        Declaration_Tracker* declaration_tracker;
        utils::Linked_List <char*>* path;
        int type;

        ~Name_Space(); Name_Space(utils::Linked_List <char*>*, int);

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_spaces_collection;

        ~Name_Space_Control(); Name_Space_Control();

        Name_Space* add_name_space(utils::Linked_List <char*>*, int);

        void add_name_space(Name_Space*);

        Name_Space* get_name_space(utils::Linked_List <char*>*);

        Name_Space* get_built_ins_name_space();

    };

    struct Type_Information {

        Ast_Node_Struct_Declaration* declaration;
        int pointer_level;

        ~Type_Information(); Type_Information(Ast_Node_Struct_Declaration*, int);

        static Type_Information* generate(Ast*, bool);

        static int get_pointer_level(Ast*);

    };


    int get_node_type(Ast*);

    utils::Linked_List <char*>* get_path(Ast*);

    utils::Linked_List <char*>* get_name_space_path(Ast*);
    
    Name_Space* get_name_space_by_path(Ast*);

    Declaration_Tracker* get_current_declaration_tracker(Ast*);

}

#endif