#include "ast_helper.h"

#include "token_definitions.h"
#include "ast_definitions.h"
#include "ast_exceptions.h"
#include "linked_list.h"
#include "built_ins.h"
#include "ast_nodes.h"
#include "common.h"
#include "token.h"
#include "ast.h"

#include <iostream>
#include <string.h>


parser::Declaration_Tracker::~Declaration_Tracker() { delete struct_declarations; delete variable_declarations; delete function_declarations; }

parser::Declaration_Tracker::Declaration_Tracker() {
    
    struct_declarations = new utils::Linked_List <Ast_Node_Struct_Declaration*>(0);
    variable_declarations = new utils::Linked_List <Ast_Node_Variable_Declaration*>(0);
    function_declarations = new utils::Linked_List <Ast_Node_Function_Declaration*>(0);

}

parser::Ast_Node_Struct_Declaration* parser::Declaration_Tracker::get_struct_declaration(char* __struct_name) {

    if (!__struct_name) return 0;

    for (int _ = 0; _ < struct_declarations->count; _++)

        if (
            struct_declarations->operator[](_)->struct_token_name && 
            !strcmp(struct_declarations->operator[](_)->struct_token_name->identifier, __struct_name)
        ) return struct_declarations->operator[](_);

    return 0;

}

parser::Ast_Node_Variable_Declaration* parser::Declaration_Tracker::get_variable_declaration(char* __variable_name) {

    if (!__variable_name) return 0;

    for (int _ = 0; _ < variable_declarations->count; _++)

        if (
            variable_declarations->operator[](_)->variable_token_name && 
            !strcmp(variable_declarations->operator[](_)->variable_token_name->identifier, __variable_name)
        ) return variable_declarations->operator[](_);

    return 0;

}



parser::Name_Space::~Name_Space() { delete path; delete declaration_tracker; }

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __path, int __type) : path(__path), type(__type)
    { declaration_tracker = new Declaration_Tracker(); }



parser::Name_Space_Control::~Name_Space_Control() { delete name_spaces_collection; }

parser::Name_Space_Control::Name_Space_Control() { 

    name_spaces_collection = new utils::Linked_List <Name_Space*>();

    utils::Linked_List <char*>* _path = new utils::Linked_List <char*>();

    add_name_space(_path, NAME_SPACE_TYPE_NAME_SPACE);

}

parser::Name_Space* parser::Name_Space_Control::add_name_space(utils::Linked_List <char*>* __path, int __type) {

    Name_Space* _name_space;

    if (_name_space = get_name_space(__path)) return _name_space;

    _name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_name_space) Name_Space(
        __path, __type
    );

    name_spaces_collection->add(_name_space);

    return _name_space;

}

void parser::Name_Space_Control::add_name_space(Name_Space* __name_space) {

    Name_Space* _name_space;

    if (_name_space = get_name_space(__name_space->path)) return;

    name_spaces_collection->add(__name_space);

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

parser::Name_Space* parser::Name_Space_Control::get_built_ins_name_space() {

    utils::Linked_List <char*>* _path = new utils::Linked_List <char*>();

    _path->add(
        (char*) "built_ins",
        0
    );

    Name_Space* _name_space = get_name_space(_path);

    delete _path;

    return _name_space;

}



parser::Type_Information::~Type_Information() {}

parser::Type_Information::Type_Information(Ast_Node_Struct_Declaration* __declaration_node, int __pointer_level) 
    : pointer_level(__pointer_level), declaration(__declaration_node) {}

parser::Type_Information* parser::Type_Information::generate(Ast* __ast, bool __pointer_level) { 

    int _inicial_position = __ast->get_token(0)->position_information.column;

    Name_Space* _name_space;

    if (_name_space = get_name_space_by_path(__ast)) __ast->add_to_chain(_name_space);

    char* _struct_name;

    if (is_primitive_type(__ast->get_token(0)->id) && !_name_space) {

        Name_Space* _built_ins_name_space = __ast->name_space_control->get_built_ins_name_space();

        if (!_built_ins_name_space) 
            throw Undefined_Built_Ins_Name_Space_Path_Ast(__ast->code_information, __ast->get_token(0));

        __ast->add_to_chain(_built_ins_name_space);

        _struct_name = built_ins::get_struct_name_of_primitive_type(__ast->get_token(0)->id);

    }

    else if (__ast->get_token(0)->id == IDENTIFIER) 
    
        _struct_name = utils::get_string_copy(
            __ast->get_token(0)->identifier
        );

    else throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0));

    Ast_Node_Struct_Declaration* _struct_declaration_node =
        get_current_declaration_tracker(__ast)->get_struct_declaration(_struct_name);

    if (!_struct_declaration_node)

        throw Undefined_Struct_Declaration_Ast(__ast->code_information, __ast->get_token(0), _inicial_position);

    __ast->tokens_position++;

    int _pointer_level = __pointer_level ? get_pointer_level(__ast) : 0;

    Type_Information* _type_information = 
        new Type_Information(
            _struct_declaration_node, _pointer_level
        );

    if (_name_space) __ast->pop_from_chain();

    free(_struct_name);

    return _type_information;

}

int parser::Type_Information::get_pointer_level(Ast* __ast) {

    int _pointer_level = 0;

    if (__ast->get_token(0)->id == FUNCTION_OPERATOR_MULTIPLICATION) __ast->get_token(0)->id = POINTER;
    
    while(__ast->get_token(0)->id == POINTER) 
        { _pointer_level++; __ast->tokens_position++; }

    return _pointer_level;

}



int parser::get_node_type(Ast* __ast) {

    int _backup_state = __ast->tokens_position;

    switch (__ast->get_token(0)->id)
    {
        // Not Ast Nodes     
        case CLOSE_BRACES: return AST_CLOSE_BRACE; break;

        // Ast Nodes
        case NAMESPACE: return AST_NODE_NAME_SPACE; break;
        case STRUCT: return AST_NODE_STRUCT_DECLARATION; break;
        case STATIC: __ast->tokens_position++; break;
        
        default: break;
    }

    if (
        is_primitive_type(__ast->get_token(0)->id) || __ast->get_token(0)->id == DOUBLE_COLON || __ast->get_token(0)->id == IDENTIFIER || __ast->get_token(0)->id == STATIC
    ) {

        delete Type_Information::generate(__ast, 1);

        get_name_space_by_path(__ast);

        __ast->tokens_position = _backup_state;

        return __ast->get_token(1)->id == OPEN_PARENTHESIS ? AST_NODE_FUNCTION_DECLARATION : AST_NODE_VARIABLE_DECLARATION;

    }

    throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0));

}

utils::Linked_List <char*>* parser::get_path(Ast* __ast) {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>();
    int _expected = IDENTIFIER;

    while(__ast->get_token(0)->id == _expected) {

        if (_expected == IDENTIFIER) {

            _scope->add(
                utils::get_string_copy(
                    __ast->get_token(0)->identifier   
                )
            );
            
            _expected = DOUBLE_COLON;

        }

        else _expected = IDENTIFIER;

        __ast->tokens_position++;

    }

    if (_expected == IDENTIFIER) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

    return _scope;

}

utils::Linked_List <char*>* parser::get_name_space_path(Ast* __ast) {

    if (
        __ast->get_token(0)->id != DOUBLE_COLON && __ast->get_token(0)->id != IDENTIFIER
    ) return NULL;

    bool _global;

    if (
        (_global = __ast->get_token(0)->id == DOUBLE_COLON)
    ) __ast->tokens_position++;

    utils::Linked_List <char*>* _path = get_path(__ast);

    if (!_global) __ast->join_with_current_path(_path);

    delete _path->remove(_path->count);

    __ast->tokens_position--;

    return _path;

}

parser::Name_Space* parser::get_name_space_by_path(Ast* __ast) {

    utils::Data_Linked_List <Token*>* _token_data_ll = 
        __ast->tokens_collection->getDataLinkedList(
            __ast->tokens_position
        );

    utils::Linked_List <char*>* _path = get_name_space_path(__ast);

    if (!_path) return NULL;

    Name_Space* _name_space = __ast->name_space_control->get_name_space(_path);

    if (!_name_space) {

        int _size = 0;

        utils::Data_Linked_List <Token*>* _current_token_data_ll = 
            __ast->tokens_collection->getDataLinkedList(
                __ast->tokens_position - 1
            );

        while(_current_token_data_ll != _token_data_ll) {

            _size++;

            _token_data_ll = _token_data_ll->next;

        }

        throw Undefined_Name_Space_Path_Ast(__ast->code_information, _current_token_data_ll, _size);

    }

    delete _path;

    return _name_space;

}

parser::Declaration_Tracker* parser::get_current_declaration_tracker(Ast* __ast) 
    { return __ast->name_space_chain->last->object->declaration_tracker; }
