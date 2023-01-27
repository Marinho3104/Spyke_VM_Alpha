#include "ast_nodes.h"

#include "ast_definitions.h"
#include "linked_list.h"
#include "ast.h"

#include <iostream>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_type) : node_type(__node_type) {}



parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE), name_space(__name_space) 
    { declarations = new utils::Linked_List <Ast_Node*>(); }

parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generate(Ast* __ast, Name_Space* __name_space) {

    __ast->print("--> Ast Node Name Space <---");

    Ast_Node_Name_Space* _node_name_space = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));

    new (_node_name_space) Ast_Node_Name_Space(__name_space);

    __ast->open_nodes->add(_node_name_space, 0);

    __ast->add_to_chain(__name_space);

    // Declarations

    __ast->pop_from_chain();

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Name Space End <---");

    return _node_name_space;

}

parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generate(Ast* __ast) {
    return 0;
}

