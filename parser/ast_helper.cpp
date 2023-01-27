#include "ast_helper.h"

#include "linked_list.h"

#include <iostream>


parser::Name_Space::~Name_Space() { delete path; }

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __path) : path(__path) {}



parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces_collection; }

parser::Name_Space_Control::Name_Space_Control() { 

    name_spaces_collection = new utils::Linked_List <Name_Space*>();

    utils::Linked_List <char*>* _path = new utils::Linked_List <char*>();

    add_name_space(_path);

}

void parser::Name_Space_Control::add_name_space(utils::Linked_List <char*>* __path) {

    if (get_name_space(__path)) return;

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_name_space) Name_Space(
        __path
    );

    name_spaces_collection->add(_name_space);

}

parser::Name_Space* parser::Name_Space_Control::get_name_space(utils::Linked_List <char*>* __path) {

    for (int _ = 0; _ < name_spaces_collection->count; _++)

        if (
            name_spaces_collection->operator[](_)->path->operator==(
                __path
            )
        ) return name_spaces_collection->operator[](_);

    return 0;

}
