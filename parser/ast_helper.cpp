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

parser::Ast_Node_Function_Declaration* parser::Declaration_Tracker::get_function_declaration(
    char* __function_name, utils::Linked_List <Type_Information*>* __parameters_types, bool __is_static) {

        Ast_Node_Function_Declaration* _function_declaration;

        for (int _ = 0; _ < function_declarations->count; _++)

            if (
                function_declarations->operator[](_)->function_token_name && 
                !strcmp(function_declarations->operator[](_)->function_token_name->identifier, __function_name)
            ) {

                _function_declaration = function_declarations->operator[](_);

                // std::cout << "Count Compare -> " << _function_declaration->parameters_type->count << " " << __parameters_types->count << std::endl;
                // std::cout << "Static Compare -> " << _function_declaration->is_static << " " << __is_static << std::endl;

                if (
                    _function_declaration->parameters_type->count != __parameters_types->count || 
                    _function_declaration->is_static != __is_static
                ) _function_declaration = 0;

                for (int _ = 0; _ < __parameters_types->count && _function_declaration; _++)

                    if (
                        __parameters_types->operator[](_)->operator!=(
                            _function_declaration->parameters_type->operator[](_)
                        )
                    ) _function_declaration = 0;

                if (_function_declaration) return _function_declaration;

            }

        return 0;

}



parser::Name_Space::~Name_Space() { delete path; delete declaration_tracker; }

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __path, int __type) : path(__path), type(__type)
    { declaration_tracker = new Declaration_Tracker(); }

utils::Linked_List <char*>* parser::Name_Space::get_previous_path() {

    if (!path->count) return 0;

    utils::Linked_List <char*>* _path = new utils::Linked_List <char*>();

    for (int _ = 0; _ < path->count - 1; _++) 
        
        _path->add(
            utils::get_string_copy(
                path->operator[](_)
            )
        );

    return _path;

}



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

    if (!__path) return 0;

    for (int _ = 0; _ < name_spaces_collection->count; _++)

        if (
            name_spaces_collection->operator[](_)->path->operator==(
                __path
            )
        ) return name_spaces_collection->operator[](_);

    return 0;

}

parser::Name_Space* parser::Name_Space_Control::get_built_ins_name_space() {

    utils::Linked_List <char*>* _path = get_built_ins_path();

    Name_Space* _name_space = get_name_space(_path);

    delete _path;

    return _name_space;

}

parser::Name_Space* parser::Name_Space_Control::get_primitive_type_name_space(Ast* __ast, int __primitive_type) {

    Name_Space* _built_ins_name_space =
        get_built_ins_name_space();

    if (!_built_ins_name_space) throw Ordinary_Exception_Ast("Built_Ins Name Space not defined");

    __ast->add_to_chain(
        _built_ins_name_space
    );

    char* _primitive_type_struct_name =
        built_ins::get_struct_name_of_primitive_type(
            __primitive_type
        );

    Ast_Node_Struct_Declaration* _primitive_type_struct_declaration =
        get_struct_declaration(__ast, _primitive_type_struct_name);

    if (!_primitive_type_struct_declaration) throw Ordinary_Exception_Ast("Primitive type struct not defined");

    __ast->pop_from_chain();

    free(_primitive_type_struct_name);

    return _primitive_type_struct_declaration->body->name_space;

}

utils::Linked_List <char*>* parser::Name_Space_Control::get_built_ins_path() {

    utils::Linked_List <char*>* _path = new utils::Linked_List <char*>();

    _path->add(
        (char*) "built_ins",
        0
    );

    return _path;

}




parser::Type_Information::~Type_Information() {}

parser::Type_Information::Type_Information(Ast_Node_Struct_Declaration* __declaration_node, int __pointer_level) 
    : pointer_level(__pointer_level), declaration(__declaration_node) {}

bool parser::Type_Information::operator==(Type_Information* __to_compare) {

    // std::cout << "--> Compare <--" << std::endl;
    // std::cout << "This -> " << declaration->struct_token_name->identifier << std::endl;
    // std::cout << "To Comp -> " << __to_compare->declaration->struct_token_name->identifier << std::endl;
    // std::cout << "This Pointer Level -> " << pointer_level << std::endl;
    // std::cout << "To Comp Pointer Level -> " << __to_compare->pointer_level << std::endl;
    // std::cout << is_pointer_equal(__to_compare) << std::endl;
    // std::cout << (__to_compare->declaration == declaration && __to_compare->pointer_level == pointer_level) << std::endl;
    // std::cout << "--> Compare End <--" << std::endl;

    return (
        is_pointer_equal(__to_compare) ||
        (__to_compare->declaration == declaration && __to_compare->pointer_level == pointer_level)
    );

}

bool parser::Type_Information::operator!=(Type_Information* __to_compare) { return !operator==(__to_compare); }

bool parser::Type_Information::is_pointer_equal(Type_Information* __to_compare) {

    return (
        (declaration->is_pointer_struct_type() && __to_compare->pointer_level) || 
        (pointer_level && __to_compare->declaration->is_pointer_struct_type())
    );

}

parser::Type_Information* parser::Type_Information::get_copy() {

    return 
        new Type_Information(
            declaration, pointer_level
        );

}

parser::Type_Information* parser::Type_Information::generate(Ast* __ast, bool __pointer_level) { 

    int _inicial_position = __ast->get_token(0)->position_information.column;

    Name_Space* _name_space;

    if (_name_space = get_name_space_by_path(__ast)) __ast->add_to_chain(_name_space);

    char* _struct_name;

    if (is_primitive_type(__ast->get_token(0)->id) && !_name_space) {

        _name_space = __ast->name_space_control->get_built_ins_name_space();

        if (!_name_space) 
            throw Undefined_Built_Ins_Name_Space_Path_Ast(__ast->code_information, __ast->get_token(0));

        __ast->add_to_chain(_name_space);

        _struct_name = built_ins::get_struct_name_of_primitive_type(__ast->get_token(0)->id);

    }

    else if (__ast->get_token(0)->id == IDENTIFIER) 
    
        _struct_name = utils::get_string_copy(
            __ast->get_token(0)->identifier
        );

    else throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0));

    Ast_Node_Struct_Declaration* _struct_declaration_node =
        get_current_declaration_tracker(__ast)->get_struct_declaration(_struct_name);

    if (!_struct_declaration_node) {

        if (_name_space) __ast->pop_from_chain();

        free(_struct_name); 

        throw Undefined_Struct_Declaration_Ast(__ast->code_information, __ast->get_token(0), _inicial_position);

    }

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

parser::Type_Information* parser::Type_Information::generate_implicit_value(Ast* __ast, int __implicit_type_id) {

    int _inicial_position = __ast->get_token(0)->position_information.column;

    Name_Space* _built_ins_name_space = __ast->name_space_control->get_built_ins_name_space();

    if (!_built_ins_name_space) 
        throw Undefined_Built_Ins_Name_Space_Path_Ast(__ast->code_information, __ast->get_token(0));

    __ast->add_to_chain(_built_ins_name_space);

    char* _struct_name = built_ins::get_struct_name_of_primitive_type(
        get_primitive_type_of_implicit_value_type(
            __implicit_type_id
        )
    );

    Ast_Node_Struct_Declaration* _struct_declaration_node =
        get_current_declaration_tracker(__ast)->get_struct_declaration(_struct_name);

    if (!_struct_declaration_node) {

        __ast->pop_from_chain();

        free(_struct_name); 

        throw Undefined_Struct_Declaration_Ast(__ast->code_information, __ast->get_token(0), _inicial_position);

    }

    Type_Information* _type_information = 
        new Type_Information(
            _struct_declaration_node, 0
        );

    __ast->pop_from_chain();

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
        case END_INSTRUCTION: return AST_END_INSTRUCTION; break;

        // Ast Nodes
        case NAMESPACE: return AST_NODE_NAME_SPACE; break;
        case OPEN_BRACES: return AST_NODE_CODE_BLOCK; break;
        case STRUCT: return AST_NODE_STRUCT_DECLARATION; break;
        case OPEN_PARENTHESIS: return AST_NODE_PARENTHESIS; break;
        case POINTER: case ADDRESS: return AST_NODE_POINTER_OPERATION; break;

        // No Return
        case STATIC: __ast->tokens_position++; break;
        
        default: break;
    }

    if (
        is_primitive_type(__ast->get_token(0)->id) || __ast->get_token(0)->id == DOUBLE_COLON || __ast->get_token(0)->id == IDENTIFIER
    ) {

        try { delete Type_Information::generate(__ast, 1); }
        
        catch (...) { 

            __ast->tokens_position = _backup_state;
            
            get_name_space_by_path(__ast); 

            int _node_type = __ast->get_token(1)->id == OPEN_PARENTHESIS ? AST_NODE_FUNCTION_CALL : AST_NODE_VARIABLE;

            __ast->tokens_position = _backup_state;

            return _node_type;

        }
        

        get_name_space_by_path(__ast);

        int _node_type = __ast->get_token(1)->id == OPEN_PARENTHESIS ? AST_NODE_FUNCTION_DECLARATION : AST_NODE_VARIABLE_DECLARATION;

        __ast->tokens_position = _backup_state;

        return _node_type;

    }

    else if (is_implicit_value_type(__ast->get_token(0)->id)) return AST_NODE_IMPLICIT_VALUE;

    throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0));

    return -1;

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
        __ast->get_token(0)->id != DOUBLE_COLON && __ast->get_token(1)->id != DOUBLE_COLON //&& __ast->get_token(0)->id != IDENTIFIER
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

parser::Ast_Node_Variable_Declaration* parser::get_variable_declaration(Ast* __ast, char* __variable_name) {

    Name_Space* _name_space = __ast->name_space_chain->last->object;
    Ast_Node_Variable_Declaration* _variable_declaration_node;
    utils::Linked_List <char*>* _previous_path;

    if (_name_space->type == NAME_SPACE_TYPE_CODE_BLOCK || _name_space->type == NAME_SPACE_TYPE_FUNCTION_BODY) {

            Ast_Node_Code_Block* _code_block = get_code_block_node(__ast, _name_space);

            while(_code_block) {

                if (
                    _variable_declaration_node = _code_block->name_space->declaration_tracker->get_variable_declaration(__variable_name)
                ) return _variable_declaration_node;

                if (!_code_block->previous) _name_space = _code_block->name_space;

                _code_block = _code_block->previous;

            }

    }

    while(_name_space) {

        if (
            _variable_declaration_node = _name_space->declaration_tracker->get_variable_declaration(__variable_name)
        ) return _variable_declaration_node;

        _previous_path = _name_space->get_previous_path();

        _name_space = __ast->name_space_control->get_name_space(_previous_path);

        delete _previous_path;

    }

    return 0;

}

parser::Ast_Node_Function_Declaration* parser::get_function_declaration(
    Ast* __ast, char* __function_name, utils::Linked_List <Type_Information*>* __parameters_type, bool __is_static) {

        Name_Space* _name_space = __ast->name_space_chain->last->object;
        Ast_Node_Function_Declaration* _function_declaration_node;
        utils::Linked_List <char*>* _previous_path;

        if (_name_space->type == NAME_SPACE_TYPE_CODE_BLOCK || _name_space->type == NAME_SPACE_TYPE_FUNCTION_BODY) {

            Ast_Node_Code_Block* _code_block = get_code_block_node(__ast, _name_space);

            while(_code_block) {

                if (
                    _function_declaration_node = 
                        _code_block->name_space->declaration_tracker->get_function_declaration(__function_name, __parameters_type, __is_static)
                ) return _function_declaration_node;

                if (!_code_block->previous) _name_space = _code_block->name_space;

                _code_block = _code_block->previous;

            }

        }

        while(_name_space) {

            if (
                _function_declaration_node = 
                    _name_space->declaration_tracker->get_function_declaration(__function_name, __parameters_type, __is_static)
            ) return _function_declaration_node;

            _previous_path = _name_space->get_previous_path();

            _name_space = __ast->name_space_control->get_name_space(_previous_path);

            delete _previous_path;

        }

        return 0;

}

parser::Ast_Node_Struct_Declaration* parser::get_struct_declaration(Ast* __ast, char* __struct_name) {

    Name_Space* _name_space = __ast->name_space_chain->last->object;
    Ast_Node_Struct_Declaration* _struct_declaration_node;
    utils::Linked_List <char*>* _previous_path;

    if (_name_space->type == NAME_SPACE_TYPE_CODE_BLOCK || _name_space->type == NAME_SPACE_TYPE_FUNCTION_BODY) {

            Ast_Node_Code_Block* _code_block = get_code_block_node(__ast, _name_space);

            while(_code_block) {

                if (
                    _struct_declaration_node = _code_block->name_space->declaration_tracker->get_struct_declaration(__struct_name)
                ) return _struct_declaration_node;

                if (!_code_block->previous) _name_space = _code_block->name_space;

                _code_block = _code_block->previous;

            }

    }

    while(_name_space) {

        if (
            _struct_declaration_node = _name_space->declaration_tracker->get_struct_declaration(__struct_name)
        ) return _struct_declaration_node;

        _previous_path = _name_space->get_previous_path();

        _name_space = __ast->name_space_control->get_name_space(_previous_path);

        delete _previous_path;

    }

    return 0;

}


parser::Ast_Node_Code_Block* parser::get_code_block_node(Ast* __ast, Name_Space* __name_space) {

    for (int _  = 0; _  < __ast->open_nodes->count; _++) 

        if (
            __ast->open_nodes->operator[](_)->node_type == AST_NODE_CODE_BLOCK &&
            ( (Ast_Node_Code_Block*) __ast->open_nodes->operator[](_) )->name_space == __name_space
        ) return ( (Ast_Node_Code_Block*) __ast->open_nodes->operator[](_) );

    throw Ordinary_Exception_Ast("Internal Error -- No Code Block found in Open Nodes");

    return 0;

}



