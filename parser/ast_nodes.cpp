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
            case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate(__ast)); break;
            
            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out:;

}

void parser::Ast_Node_Name_Space::set_declarations_struct_body(Ast* __ast) {

    int _backup_state = __ast->tokens_position;
    int node_type;

    __ast->print("--> Ast Node Struct Declaration -- Fields <--");

    __ast->open_nodes->add(0, 0);

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (node_type = get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out_fields; break;

            case AST_NODE_VARIABLE_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Variable_Declaration::generate(__ast); declarations->join(_); delete _; break; }
            case AST_NODE_FUNCTION_DECLARATION: Ast_Node_Function_Declaration::ignore(__ast); break;

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out_fields:;

    __ast->tokens_position = _backup_state;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Struct Declaration End -- Fields <--");



    __ast->print("--> Ast Node Struct Declaration -- Functions <--");

    __ast->open_nodes->add(0, 0);

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (node_type = get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out_functions; break;

            case AST_NODE_VARIABLE_DECLARATION: Ast_Node_Variable_Declaration::ignore(__ast); break;
            case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate(__ast)); break;

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out_functions:;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Struct Declaration End -- Functions <--");


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



parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { if (name_space->type == NAME_SPACE_TYPE_CODE_BLOCK) delete name_space; delete code; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Ast_Node_Code_Block* __previous, Name_Space* __name_space) 
    : Ast_Node(AST_NODE_CODE_BLOCK, 0), previous(__previous), name_space(__name_space) 
        { code = new utils::Linked_List <Ast_Node*>(); }

void parser::Ast_Node_Code_Block::set_code(Ast* __ast) {

    int _node_type;

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (_node_type = get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out; break;

            case AST_NODE_CODE_BLOCK: code->add(Ast_Node_Code_Block::generate(__ast)); break;
            case AST_NODE_VARIABLE_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Variable_Declaration::generate(__ast); code->join(_); delete _; break; }
            case AST_NODE_VARIABLE: case AST_NODE_FUNCTION_CALL:
                code->add(Ast_Node_Expression::generate(__ast, _node_type)); break;

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out:;

}

parser::Ast_Node_Code_Block* parser::Ast_Node_Code_Block::generate(Ast* __ast) {

    __ast->print("--> Ast Node Code Block <---");

    if (
        __ast->open_nodes->last->object->node_type != AST_NODE_CODE_BLOCK
    ) throw Ordinary_Exception_Ast("Internal error: Expected AST_NODE_CODE_BLOCK");

    Name_Space* _name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _code_block_node = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_code_block_node) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _name_space
    );

    __ast->tokens_position++;

    __ast->add_to_chain(
        _name_space
    );

    __ast->open_nodes->add(
        _code_block_node, 0
    );

    _code_block_node->set_code(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node Code Block End <---");

    return _code_block_node;

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

        __ast->add_to_chain(
            _struct_declaration->body->name_space
        );

        _struct_declaration->body->set_declarations_struct_body(__ast);

        __ast->pop_from_chain();

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

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate_function_parameters(Ast* __ast) {

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;

    Type_Information* _variable_type;
    Token* _variable_token_name;

    while(__ast->get_token(0)->id != CLOSE_PARENTHESIS) {

        __ast->print("--> Ast Node Variable Declaration -- Function Parameters <--");

        _variable_type = Type_Information::generate(__ast, 1);

        _variable_token_name = __ast->get_token(0)->id == IDENTIFIER ? __ast->get_token(0) : 0;

        if (_variable_token_name) {

            __ast->tokens_position++;

            if (
                Ast_Node_Variable_Declaration* _previous_variable_declaration_node = 
                    __ast->name_space_chain->last->object->declaration_tracker->get_variable_declaration(
                        _variable_token_name->identifier
                    )
            ) throw Redefinition_Variable_Declaration_Ast(
                __ast->code_information, 
                _previous_variable_declaration_node->variable_token_name, 
                _variable_token_name
            );

        }

        Ast_Node_Variable_Declaration* _variable_declaration_node = 
            (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

        new (_variable_declaration_node) Ast_Node_Variable_Declaration(
            _variable_type, _variable_token_name, 0
        );

        _nodes->add(
            _variable_declaration_node
        );

        __ast->name_space_chain->last->object->declaration_tracker->variable_declarations->add(
            _variable_declaration_node
        );
        
        __ast->print("--> Ast Node Variable Declaration End -- Function Parameters <--");

        switch (__ast->get_token(0)->id)
        {
        case COMMA: __ast->tokens_position++; break;
        case CLOSE_PARENTHESIS: break; 
        default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

    __ast->tokens_position++;

    return _nodes;

}

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

void parser::Ast_Node_Variable_Declaration::ignore(Ast* __ast) 
    { while (__ast->get_token(0)->id != END_INSTRUCTION) __ast->tokens_position++; __ast->tokens_position++; }



parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() 
    { delete representive_type; 
    delete parameters_type; 
    body->~Ast_Node_Code_Block(); 
    free(body); 
    name_space->~Name_Space(); 
    free(name_space); }

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(Type_Information* __return_type, Token* __function_token_name, Name_Space* __name_space, bool __is_static) 
    : Ast_Node(AST_NODE_FUNCTION_DECLARATION, __return_type), function_token_name(__function_token_name), name_space(__name_space), is_static(__is_static) { 

        body = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));  new (body) Ast_Node_Code_Block(0, __name_space); 

        parameters_type = new utils::Linked_List <Type_Information*>(0);

    }

void parser::Ast_Node_Function_Declaration::set_this_variable(Ast* __ast) {

    // std::cout << "Setting this variable" << std::endl;

    char* _struct_name = __ast->name_space_chain->last->object->path->last->object;

    utils::Linked_List <char*>* _previous_path = __ast->name_space_chain->last->object->get_previous_path();

    __ast->add_to_chain(
        __ast->name_space_control->get_name_space(
            _previous_path
        )
    );

    Ast_Node_Struct_Declaration* _struct_declaration_node =
        get_current_declaration_tracker(__ast)->get_struct_declaration(
            _struct_name
        );

    __ast->pop_from_chain();
    
    parameters_type->add(
        _struct_declaration_node->representive_type->get_copy()
    );

    parameters_type->first->object->pointer_level++;

    Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (_variable_declaration_node) Ast_Node_Variable_Declaration( // Add name
        parameters_type->first->object,
        0, 0
    );

    body->code->add(
        _variable_declaration_node
    );

    name_space->declaration_tracker->variable_declarations->add(
        _variable_declaration_node
    );

    delete _previous_path;

}

void parser::Ast_Node_Function_Declaration::set_parameters(Ast* __ast) {

    __ast->tokens_position++;

    __ast->add_to_chain(
        name_space
    );

    utils::Linked_List <Ast_Node*>* _parameters = 
        Ast_Node_Variable_Declaration::generate_function_parameters(__ast);

    __ast->pop_from_chain();

    body->code->join(_parameters);

    for (int _ = 0; _ < _parameters->count; _++) 

        if (_parameters->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION) 

            parameters_type->add(
                _parameters->operator[](_)->representive_type
            );

    delete _parameters;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast* __ast) {

    __ast->print("--> Ast Node Function Declaration <--");

    Token* _static_token = 0;

    if (__ast->get_token(0)->id == STATIC) 
        { _static_token = __ast->get_token(0); __ast->tokens_position++; }

    Type_Information* _return_type = Type_Information::generate(__ast, 1);

    Name_Space* _name_space = get_name_space_by_path(__ast);

    if (_name_space) __ast->add_to_chain(_name_space);

    if (__ast->name_space_chain->last->object->type != NAME_SPACE_TYPE_STRUCT_BODY && _static_token) 
        throw Unexpected_Token_Ast(__ast->code_information, _static_token);

    Token* _function_token_name = __ast->get_token(0);
    __ast->tokens_position++;

    utils::Linked_List <char*>* _function_path = 
        new utils::Linked_List <char*>();

    _function_path->add(
        utils::get_string_copy(
            _function_token_name->identifier
        )
    );

    __ast->join_with_current_path(_function_path);

    Name_Space* _function_name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_function_name_space) Name_Space(
        _function_path, NAME_SPACE_TYPE_FUNCTION_BODY
    );

    Ast_Node_Function_Declaration* _function_declaration_node = (Ast_Node_Function_Declaration*) malloc(sizeof(Ast_Node_Function_Declaration));

    new (_function_declaration_node) Ast_Node_Function_Declaration(
        _return_type, _function_token_name, _function_name_space, _static_token
    );

    __ast->open_nodes->add(
        _function_declaration_node, 0
    );

    if (__ast->name_space_chain->last->object->type == NAME_SPACE_TYPE_STRUCT_BODY && !_static_token)
        
        _function_declaration_node->set_this_variable(__ast);

    _function_declaration_node->set_parameters(__ast);

    if (
        Ast_Node_Function_Declaration* _previous_function_declaration_node = 
            __ast->name_space_chain->last->object->declaration_tracker->get_function_declaration(
                _function_token_name->identifier, _function_declaration_node->parameters_type, _static_token
            )
    )
        throw Redefinition_Function_Declaration_Ast(
            __ast->code_information,
            _previous_function_declaration_node->function_token_name,
            _function_token_name
        );

    get_current_declaration_tracker(__ast)->function_declarations->add(
        _function_declaration_node
    );

    __ast->tokens_position++;

    __ast->add_to_chain(
        _function_name_space
    );

    __ast->open_nodes->add(
        _function_declaration_node->body, 0
    );

    _function_declaration_node->body->set_code(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    if (_name_space) __ast->pop_from_chain();

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Function Declaration End <--");

    return _function_declaration_node;

}

void parser::Ast_Node_Function_Declaration::ignore(Ast* __ast) {

    while (__ast->get_token(0)->id != END_INSTRUCTION && __ast->get_token(0)->id != OPEN_BRACES) __ast->tokens_position++;

    int _internal_count = 0;

    do {

        if (__ast->get_token(0)->id == OPEN_BRACES) _internal_count++;

        else if (__ast->get_token(0)->id == CLOSE_BRACES) _internal_count--;

        __ast->tokens_position++;

    } while(_internal_count);

}



parser::Ast_Node_Expression::~Ast_Node_Expression() {}

parser::Ast_Node_Expression::Ast_Node_Expression() : Ast_Node(AST_NODE_EXPRESSION, 0) {}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast* __ast, int __node_type) {

    __ast->print("--> Ast Node Expression <--");

    std::cout << "Expression node type -> " << __node_type << std::endl;

    __ast->print("--> Ast Node Expression End <--");

    exit(1);

}

