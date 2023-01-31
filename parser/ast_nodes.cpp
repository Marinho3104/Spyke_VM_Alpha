#include "ast_nodes.h"

#include "token_definitions.h"
#include "ast_definitions.h"
#include "ast_exceptions.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "common.h"
#include "token.h"
#include "ast.h"

#include <iostream>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_type, Type_Information* __representive_type) 
    : node_type(__node_type), representive_type(__representive_type) {}



parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, 0), name_space(__name_space) 
    { declarations = new utils::Linked_List <Ast_Node*>(); }

void parser::Ast_Node_Name_Space::set_declarations(Ast* __ast) {

    int node_type;

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (node_type = get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out; break;

            case AST_NODE_NAME_SPACE: declarations->add(Ast_Node_Name_Space::generate(__ast)); break;
            case AST_NODE_STRUCT_DECLARATION: declarations->add(Ast_Node_Struct_Declaration::generate(__ast)); break;
            case AST_NODE_VARIABLE_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Variable_Declaration::generate(__ast); declarations->join(_); delete _; break; }
            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out:;

}

void parser::Ast_Node_Name_Space::set_declarations_struct_body(Ast* __ast) {

    int node_type;

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (node_type = get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out; break;

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out:;

}


parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generate(Ast* __ast, Name_Space* __name_space) {

    __ast->print("--> Ast Node Name Space <---");

    Ast_Node_Name_Space* _node_name_space = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));

    new (_node_name_space) Ast_Node_Name_Space(__name_space);

    __ast->open_nodes->add(_node_name_space, 0);

    __ast->add_to_chain(__name_space);

    _node_name_space->set_declarations(__ast);

    __ast->pop_from_chain();

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Name Space End <---");

    return _node_name_space;

}

parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generate(Ast* __ast) {

    __ast->tokens_position++;

    utils::Linked_List <char*>* _path = get_path(__ast);

    __ast->join_with_current_path(_path);

    Name_Space* _name_space = __ast->name_space_control->add_name_space(_path, NAME_SPACE_TYPE_NAME_SPACE);

    if (__ast->get_token(0)->id != OPEN_BRACES) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), OPEN_BRACES);

    __ast->tokens_position++;

    return generate(__ast, _name_space);
    
}



parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() { body->~Ast_Node_Name_Space(); free(body); delete representive_type; }

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(Token* __struct_token_name, Name_Space* __struct_name_space) 
    : Ast_Node(AST_NODE_STRUCT_DECLARATION, 0), struct_token_name(__struct_token_name), body_defined(0) {

            representive_type = new Type_Information(this, 0);

            body = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space)); new (body) Ast_Node_Name_Space(__struct_name_space); 

    }

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate(Ast* __ast) {

    __ast->print("--> Ast Node Struct Declaration <---");

    __ast->tokens_position++;

    Token* _struct_token_name = 0;

    if (__ast->get_token(0)->id == IDENTIFIER) {

        _struct_token_name = __ast->get_token(0);

        __ast->tokens_position++;
        
    }

    Name_Space* _struct_name_space = 0;

    Ast_Node_Struct_Declaration* _previous_struct_declaration =
        __ast->name_space_chain->last->object->declaration_tracker->get_struct_declaration(
            _struct_token_name ? _struct_token_name->identifier : 0
        );

    if (_previous_struct_declaration) {

        if (_previous_struct_declaration->body_defined)

            throw Redefinition_Struct_Declaration_Ast(
                __ast->code_information, 
                _previous_struct_declaration->struct_token_name, 
                _struct_token_name
            );

        _struct_name_space = 
            _previous_struct_declaration->body->name_space;

    }

    else {

        utils::Linked_List <char*>* _struct_path = new utils::Linked_List <char*>();

        _struct_path->add(
            utils::get_string_copy(
                _struct_token_name->identifier
            )
        );

        __ast->join_with_current_path(_struct_path);

        _struct_name_space = (Name_Space*) malloc(sizeof(Name_Space));

        new (_struct_name_space) Name_Space(
            _struct_path, NAME_SPACE_TYPE_STRUCT_BODY
        );

        __ast->name_space_control->add_name_space(_struct_name_space);

    }

    Ast_Node_Struct_Declaration* _struct_declaration = 
        (Ast_Node_Struct_Declaration*) malloc(sizeof(Ast_Node_Struct_Declaration));
 
    new (_struct_declaration) Ast_Node_Struct_Declaration(
        _struct_token_name, _struct_name_space
    );

    __ast->open_nodes->add(
        _struct_declaration, 0
    );

    get_current_declaration_tracker(__ast)->struct_declarations->add(
        _struct_declaration
    );

    __ast->tokens_position++;

    switch (__ast->get_token(-1)->id)
    {
    case OPEN_BRACES: 

        _struct_declaration->body->set_declarations_struct_body(__ast);
        _struct_declaration->body_defined = 1;

        __ast->tokens_position++;
    
        break;

    case END_INSTRUCTION: break;
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
    }

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Struct Declaration End <---");

    return _struct_declaration;

}



parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete representive_type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(Type_Information* __type, Token* __variable_token_name, bool __is_static)
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION, __type), variable_token_name(__variable_token_name), is_static(__is_static) {}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Variable_Declaration::generate(
    Ast* __ast, Ast_Node_Struct_Declaration* __struct_declaration_node, bool __is_static) {

        __ast->print("--> Ast Node Variable Declaration <---");
        
        Type_Information* _type = 
            new Type_Information(
                __struct_declaration_node,
                Type_Information::get_pointer_level(__ast)
            );

        if (__ast->get_token(0)->id != IDENTIFIER) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

        if (
            Ast_Node_Variable_Declaration* _previous_variable_declaration_node = 
                __ast->name_space_chain->last->object->declaration_tracker->get_variable_declaration(
                    __ast->get_token(0)->identifier
                )
        ) throw Redefinition_Variable_Declaration_Ast(
            __ast->code_information, 
            _previous_variable_declaration_node->variable_token_name, 
            __ast->get_token(0)
        );

        Ast_Node_Variable_Declaration* _variable_declaration_node = 
            (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

        new (_variable_declaration_node) Ast_Node_Variable_Declaration(
            _type, __ast->get_token(0), __is_static
        );

        __ast->tokens_position++;

        __ast->name_space_chain->last->object->declaration_tracker->variable_declarations->add(
            _variable_declaration_node
        );

        __ast->print("--> Ast Node Variable Declaration End <---");

        return _variable_declaration_node;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate(Ast* __ast) {

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;

    bool _is_static;

    if (
        _is_static = __ast->get_token(0)->id == STATIC
    ) __ast->tokens_position++;

    Type_Information* _type = Type_Information::generate(__ast, 0);

    while(__ast->get_token(0)->id != END_INSTRUCTION) {

        _nodes->add(
            generate(__ast, _type->declaration, _is_static)
        );

        switch (__ast->get_token(0)->id)
        {
        case COMMA: __ast->tokens_position++; break;
        case END_INSTRUCTION: break; 
        default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

    __ast->tokens_position++;

    delete _type;

    return _nodes;

}





