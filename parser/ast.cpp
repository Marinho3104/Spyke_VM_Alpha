#include "ast.h"

#include "code_information.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "ast_nodes.h"
#include "common.h"
#include "token.h"

#include <iostream>


parser::Ast::~Ast() { 

    global_name_space->~Ast_Node_Name_Space(); free(global_name_space);

    delete name_space_control; 

    // std::cout << "Name Space Count -> " << name_space_chain->count << std::endl;
    delete name_space_chain;
    delete implicit_values;

    // Check if open nodes is empty    
    delete open_nodes; 


}

parser::Ast::Ast(Code_Information* __code_information, utils::Linked_List <Token*>* __tokens_collection) 
    : tokens_collection(__tokens_collection), code_information(__code_information), tokens_position(0) { 
        
        name_space_chain = new utils::Linked_List <Name_Space*>();
        implicit_values = new utils::Linked_List <Token*>(0);
        open_nodes = new utils::Linked_List <Ast_Node*>();
        name_space_control = new Name_Space_Control();

        generate_ast_nodes(); 
        
}

void parser::Ast::print(const char* __information) {

    if (1) return;

    for (int _  = 0; _ < open_nodes->count + 1; _++) std::cout << "\t";

    std::cout << __information << "\n" << std::endl;

}

parser::Token* parser::Ast::get_token(int __off) { return tokens_collection->operator[](tokens_position + __off); }

void parser::Ast::generate_ast_nodes() 
    { global_name_space = Ast_Node_Name_Space::generate(this, name_space_control->name_spaces_collection->first->object); }

int parser::Ast::add_implicit_value(Token* __token) {

    for (int _ = 0; _ < implicit_values->count; _++)

        if (
            implicit_values->operator[](_)->operator==(
                __token
            )
        ) return _;

    implicit_values->add(
        __token
    );

    return implicit_values->count - 1;

}

void parser::Ast::add_to_chain(Name_Space* __name_space) { name_space_chain->add(__name_space, 0); }

void parser::Ast::pop_from_chain() { delete name_space_chain->remove(name_space_chain->count); }

void parser::Ast::join_with_current_path(utils::Linked_List <char*>* __path) {

    for (int _ = 0; _ < name_space_chain->last->object->path->count; _++)

        __path->insert(
            utils::get_string_copy(
                name_space_chain->last->object->path->operator[](_)
            ),
            _
        );

}


