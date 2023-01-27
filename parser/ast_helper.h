#pragma once 

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

#include "parser_definitions.h"

namespace parser {

    struct Name_Space {

        utils::Linked_List <char*>* path;

        ~Name_Space(); Name_Space(utils::Linked_List <char*>*);

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_spaces_collection;

        ~Name_Space_Control(); Name_Space_Control();

        void add_name_space(utils::Linked_List <char*>*);

        Name_Space* get_name_space(utils::Linked_List <char*>*);

    };

}

#endif