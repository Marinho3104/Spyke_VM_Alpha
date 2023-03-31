#include "ast_nodes.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "ast_definitions.h"
#include "ast_exceptions.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "built_ins.h"
#include "common.h"
#include "token.h"
#include "ast.h"

#include <iostream>
#include <string.h>
#include <unistd.h>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_type, Type_Information* __representive_type) 
    : node_type(__node_type), representive_type(__representive_type) {}



parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, 0), name_space(__name_space) 
    { declarations = new utils::Linked_List <Ast_Node*>(); }

void parser::Ast_Node_Name_Space::set_declarations(Ast* __ast) {

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out; break;
            case AST_END_INSTRUCTION: __ast->tokens_position++; goto out; break;

            case AST_NODE_NAME_SPACE: declarations->add(Ast_Node_Name_Space::generate(__ast)); break;
            case AST_NODE_STRUCT_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Struct_Declaration::generate(__ast); declarations->join(_); delete _; break; }
            case AST_NODE_VARIABLE_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Variable_Declaration::generate(__ast); declarations->join(_); delete _; break; }
            case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate(__ast)); break;
            case AST_NODE_BYTE_CODE: declarations->add(Ast_Node_Byte_Code::generate(__ast)); break;
            case AST_NODE_CONTRACT_DECLARATION: declarations->add(Ast_Node_Contract_Declaration::generate(__ast)); break;

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out:;

}

void parser::Ast_Node_Name_Space::set_declarations_struct_body(Ast* __ast, bool& __body_defined) {

    int _backup_state = __ast->tokens_position;

    __ast->print("--> Ast Node Struct Declaration -- Fields <--");

    __ast->open_nodes->add(0, 0);

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        // std::cout << "Node type -> " << get_node_type(__ast) << std::endl;

        switch (get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out_fields; break;

            case AST_NODE_VARIABLE_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Variable_Declaration::generate(__ast); declarations->join(_); delete _; break; }
            case AST_NODE_FUNCTION_DECLARATION: case AST_NODE_CONSTRUCTOR_CALL: case AST_BITWISE_NOT:
                Ast_Node_Function_Declaration::ignore(__ast); break; 

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out_fields:;

    bool _constructor_defined = 0, _default_destructor_defined = 0;

    __body_defined = 1;

    __ast->tokens_position = _backup_state;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Struct Declaration End -- Fields <--");



    __ast->print("--> Ast Node Struct Declaration -- Functions <--");

    __ast->open_nodes->add(0, 0);

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (int _node_type = get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out_functions; break;

            case AST_NODE_VARIABLE_DECLARATION: Ast_Node_Variable_Declaration::ignore(__ast); break;
            case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate(__ast)); break;
            case AST_NODE_CONSTRUCTOR_CALL: case AST_BITWISE_NOT: 
                declarations->add(
                    Ast_Node_Function_Declaration::generate(
                        __ast,
                        _node_type == AST_NODE_CONSTRUCTOR_CALL
                    )
                );
                
                if (_node_type == AST_NODE_CONSTRUCTOR_CALL) _constructor_defined = 1;

                else if (
                    _node_type == AST_BITWISE_NOT && 
                    ((Ast_Node_Function_Declaration*) declarations->last->object)->parameters_type->count == 1
                ) _default_destructor_defined = 1;
                
                break;
            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

out_functions:;


    if (!_constructor_defined) 

        ( (Ast_Node_Struct_Declaration*) __ast->open_nodes->last->previous->object )->add_special_function(__ast, 1);

    if (!_default_destructor_defined)

        ( (Ast_Node_Struct_Declaration*) __ast->open_nodes->last->previous->object )->add_special_function(__ast, 0);

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

void parser::Ast_Node_Code_Block::set_code(Ast* __ast, bool __once = 0) {

    utils::Linked_List <Ast_Node*>* _previous_current_nodes = __ast->current_nodes;

    __ast->current_nodes = code;

    while(__ast->tokens_position < __ast->tokens_collection->count) {

        switch (get_node_type(__ast))
        {
            case AST_CLOSE_BRACE: __ast->tokens_position++; goto out; break;
            case AST_END_INSTRUCTION: __ast->tokens_position++; break;

            case AST_NODE_CODE_BLOCK: code->add(Ast_Node_Code_Block::generate(__ast)); break;
            case AST_NODE_VARIABLE_DECLARATION: 
                { utils::Linked_List <Ast_Node*>* _ = Ast_Node_Variable_Declaration::generate(__ast); code->join(_); delete _; break; }
            case AST_NODE_EXPRESSION: case AST_NODE_VARIABLE: case AST_NODE_FUNCTION_CALL: case AST_NODE_IMPLICIT_VALUE: case AST_NODE_TYPE_CONVERSION:
                case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS: case AST_NODE_CONSTRUCTOR_CALL: case AST_NODE_CAST:
                
                    code->add(Ast_Node_Expression::generate(__ast)); 
                    
                    if (__ast->get_token(0)->id != END_INSTRUCTION) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), END_INSTRUCTION);
                    
                    __ast->tokens_position++; break;
            case AST_NODE_BYTE_CODE: code->add(Ast_Node_Byte_Code::generate(__ast)); break;
            case AST_NODE_RETURN_KEY_WORD: code->add(Ast_Node_Return_Key_Word::generate(__ast)); break;
            case AST_NODE_WHILE: code->add(Ast_Node_While::generate(__ast)); break;
            case AST_NODE_DO_WHILE: code->add(Ast_Node_Do_While::generate(__ast)); break;
            case AST_NODE_IF: code->add(Ast_Node_If::generate(__ast)); break;
            case AST_NODE_ELSE_IF: code->add(Ast_Node_Else_If::generate(__ast)); break;
            case AST_NODE_ELSE: code->add(Ast_Node_Else::generate(__ast)); break;
            case AST_NODE_FOR: code->add(Ast_Node_For::generate(__ast)); break;
            case AST_NODE_CONTROL_STRUCTS_KEY_WORD: code->add(Ast_Node_Control_Structs_Key_Words::generate(__ast)); break;

            default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

        if (__once) break;

    }

out:;

    __ast->current_nodes = _previous_current_nodes;


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



parser::Ast_Node_Contract_Declaration::~Ast_Node_Contract_Declaration() { body->~Ast_Node_Name_Space(); free(body); }

parser::Ast_Node_Contract_Declaration::Ast_Node_Contract_Declaration(Ast_Node_Name_Space* __body) 
    : Ast_Node(AST_NODE_CONTRACT_DECLARATION, 0), body(__body) {}

parser::Ast_Node_Contract_Declaration* parser::Ast_Node_Contract_Declaration::generate(Ast* __ast) {

    __ast->print("--> Ast Node Contract Declaration <--");

    utils::Linked_List <Ast_Node*>* _struct_declaration = 
        Ast_Node_Struct_Declaration::generate(__ast);

    _struct_declaration->destroy_content = 1;

    Ast_Node_Contract_Declaration* _contract_declaration_node =     
        (Ast_Node_Contract_Declaration*) malloc(sizeof(Ast_Node_Contract_Declaration));

    new (_contract_declaration_node) Ast_Node_Contract_Declaration(
        ( (Ast_Node_Struct_Declaration*) _struct_declaration->first->object)->body
    );

    (( Ast_Node_Struct_Declaration*) _struct_declaration->first->object)->body = 0;

    delete _struct_declaration;

    __ast->print("--> Ast Node Contract Declaration End <--");

    return _contract_declaration_node;

}



parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() { if (body) body->~Ast_Node_Name_Space(); free(body); delete representive_type; }

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(Token* __struct_token_name, Name_Space* __struct_name_space) 
    : Ast_Node(AST_NODE_STRUCT_DECLARATION, 0), struct_token_name(__struct_token_name), body_defined(0) {

            representive_type = new Type_Information(this, 0);

            body = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space)); new (body) Ast_Node_Name_Space(__struct_name_space); 

    }

void parser::Ast_Node_Struct_Declaration::add_special_function(Ast* __ast, bool __is_constructor) {

    __ast->print("--> Adding Special Function <--");

    // std::cout << "Struct name -> " << struct_token_name->identifier << std::endl;

    utils::Linked_List <char*>* _function_path = 
        new utils::Linked_List <char*>();

    _function_path->add(
        utils::get_string_copy(
            struct_token_name->identifier
        )
    );

    __ast->join_with_current_path(_function_path);

    Name_Space* _function_name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_function_name_space) Name_Space(
        _function_path, NAME_SPACE_TYPE_FUNCTION_BODY
    );

    Ast_Node_Function_Declaration* _function_declaration_node = (Ast_Node_Function_Declaration*) malloc(sizeof(Ast_Node_Function_Declaration));

    new (_function_declaration_node) Ast_Node_Function_Declaration(
        Type_Information::generate_primitive_type(__ast, PRIMITIVE_TYPE_VOID),
        struct_token_name,
        _function_name_space,
        0
    );

    _function_declaration_node->set_this_variable(__ast);

    _function_declaration_node->destructor = !__is_constructor;

    body->declarations->add(
        _function_declaration_node
    );

    body->name_space->declaration_tracker->function_declarations->add(
        _function_declaration_node
    );

    __ast->print("--> Adding Special Function End <--");

}

bool parser::Ast_Node_Struct_Declaration::is_pointer_struct_type() {

    utils::Linked_List <char*>* _built_ins_path = 
        Name_Space_Control::get_built_ins_path();

    utils::Linked_List <char*>* _prev_copied_path =
        body->name_space->path->get_copy(0);

    utils::Data_Linked_List <char*>* _to_remove_data_ll =
        _prev_copied_path->remove(
            _prev_copied_path->count
        );

    _to_remove_data_ll->destroy_content = 0;

    delete _to_remove_data_ll;

    bool _path_confirmation =
        _built_ins_path->operator==(
            _prev_copied_path
        );

    delete _built_ins_path;
    delete _prev_copied_path;

    return (
        _path_confirmation &&
        !strcmp( (const char*) struct_token_name->identifier, PRIMITIVE_TYPE_POINTER_NAME)
    );

}

int parser::Ast_Node_Struct_Declaration::get_variable_off(Ast_Node_Variable_Declaration* __variable) {

    int _off = 0;

    for (int _ = 0; _ < body->declarations->count; _++)

        if (
            body->declarations->operator[](_) == __variable
        ) return _off;

        else if (body->declarations->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION) 

            _off += body->declarations->operator[](_)->representive_type->get_size(); 

    return -1;

}

int parser::Ast_Node_Struct_Declaration::get_size() {

    int _size = 
        built_ins::get_primitive_type_size(
            struct_token_name->identifier
        );

    utils::Linked_List <char*>* _built_ins_path =
        Name_Space_Control::get_built_ins_path();

    body->name_space->path->count--;

    bool _path_confirmation =
        body->name_space->path->operator==(
            _built_ins_path
        );

    body->name_space->path->count++;

    delete _built_ins_path;

    if (_size != -1 && _path_confirmation) return _size;

    _size = 0;

    for (int _ = 0; _ < body->declarations->count; _++) 

        if (
            body->declarations->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION &&
                ! ( (Ast_Node_Variable_Declaration*) body->declarations->operator[](_) )->is_static
        ) 
            _size += (
                (Ast_Node_Variable_Declaration*) body->declarations->operator[](_)
            )->representive_type->get_size();

    return _size;

}

utils::Linked_List <parser::Ast_Node*>*  parser::Ast_Node_Struct_Declaration::generate(Ast* __ast) {

    __ast->print("--> Ast Node Struct Declaration <---");

    utils::Linked_List <Ast_Node*>* _nodes = 
        new utils::Linked_List <Ast_Node*>(0);

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

        if (_previous_struct_declaration->body_defined && __ast->get_token(0)->id != END_INSTRUCTION)

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

    _nodes->add(
        _struct_declaration
    );

    __ast->open_nodes->add(
        _struct_declaration, 0
    );

    get_current_declaration_tracker(__ast)->struct_declarations->add(
        _struct_declaration
    );

    switch (__ast->get_token(0)->id)
    {
    case OPEN_BRACES: 

        __ast->tokens_position++;

        __ast->add_to_chain(
            _struct_declaration->body->name_space
        );

        _struct_declaration->body->set_declarations_struct_body(__ast, _struct_declaration->body_defined);

        __ast->pop_from_chain();

        _struct_declaration->body_defined = 1;
    
        break;

    case END_INSTRUCTION: break;
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
    }

    if (__ast->get_token(0)->id != END_INSTRUCTION) {

        utils::Linked_List <Ast_Node*>* _variables_declarations =
            generate_variable_declarations(__ast, _struct_declaration);

        _nodes->join(
            _variables_declarations
        );

        delete _variables_declarations;

    }

    else __ast->tokens_position++;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Struct Declaration End <---");

    return _nodes;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Struct_Declaration::generate_variable_declarations(
    Ast* __ast, Ast_Node_Struct_Declaration* __struct_declaration) {

        __ast->print("--> Ast Node Struct Declaration -- Variable Declarations <--");

        __ast->open_nodes->add(0,0);

        utils::Linked_List <Ast_Node*>* _variable_declarations = 
            Ast_Node_Variable_Declaration::generate_struct_declaration_variables(__ast, __struct_declaration);

        delete __ast->open_nodes->remove(
            __ast->open_nodes->count
        );

        __ast->print("--> Ast Node Struct Declaration End -- Variable Declarations <--");

        return _variable_declarations;

}



parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() 
    { delete representive_type; if (constructor_call) constructor_call->~Ast_Node_Constructor_Call(); free(constructor_call);
        if (array_size) array_size->~Ast_Node_Constructor_Call(); free(array_size); }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(Type_Information* __type, Token* __variable_token_name, bool __is_static)
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION, __type), variable_token_name(__variable_token_name), is_static(__is_static), constructor_call(0), array_size(0) {}

void parser::Ast_Node_Variable_Declaration::set_array_size_expression(Ast* __ast) {

    if (
        __ast->get_token(0)->id != OPEN_BRACKET
    ) return;

    __ast->tokens_position ++;

    representive_type->pointer_level ++;

    Type_Information* _type_pointer = 
        Type_Information::generate_primitive_type(__ast, PRIMITIVE_TYPE_POINTER);

    array_size = 
        Ast_Node_Constructor_Call::generate(
            __ast,
            _type_pointer,
            Ast_Node_Expression::generate(__ast)
        );

    if (
        __ast->get_token(0)->id != CLOSE_BRACKET
    ) 
        throw Expected_Token_Ast(
            __ast->code_information,
            __ast->get_token(0), CLOSE_BRACKET
        );

    __ast->tokens_position ++;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate_struct_declaration_variables(
    Ast* __ast, parser::Ast_Node_Struct_Declaration* __struct_declaration) {

        utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;

        bool _is_static;

        if (
            _is_static = __ast->get_token(0)->id == STATIC
        ) __ast->tokens_position++;

        while(__ast->get_token(0)->id != END_INSTRUCTION) {

            _nodes->add(
                generate(__ast, __struct_declaration, _is_static, 0)
            );

            switch (__ast->get_token(0)->id)
            {
                case COMMA: __ast->tokens_position++; break;
                case END_INSTRUCTION: break; 
                default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
            }

        }

        __ast->tokens_position++;

        return _nodes;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate_function_parameters(Ast* __ast) {

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;

    Type_Information* _variable_type;
    Token* _variable_token_name;

    while(__ast->get_token(0)->id != CLOSE_PARENTHESIS) {

        __ast->print("--> Ast Node Variable Declaration -- Function Parameters <--");

        _variable_type = Type_Information::generate_confirm_body(__ast, 1);

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

        _variable_declaration_node->set_array_size_expression(__ast);

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
    Ast* __ast, Ast_Node_Struct_Declaration* __struct_declaration_node, bool __is_static, int __inicial_position) {

        __ast->print("--> Ast Node Variable Declaration <---");
        
        Type_Information* _type = 
            new Type_Information(
                __struct_declaration_node,
                Type_Information::get_pointer_level(__ast)
            );

        if (!_type->pointer_level && !__struct_declaration_node->body_defined)

            throw Struct_Body_Not_Defined_Ast(__ast->code_information, __ast->get_token(-1), __inicial_position);

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

        _variable_declaration_node->set_array_size_expression(__ast);

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

    Type_Information* _type = Type_Information::generate_confirm_body(__ast, 0);

    while(__ast->get_token(0)->id != END_INSTRUCTION) {

        _nodes->add(
            generate(__ast, _type->declaration, _is_static, _type->inicial_position)
        );

        __ast->open_nodes->add(
            _nodes->last->object, 0
        );

        if (__ast->get_token(0)->id == FUNCTION_OPERATOR_EQUAL) {

            __ast->tokens_position++;

            
            Ast_Node_Constructor_Call::set_variable_declaration_constructor(
                __ast,
                Ast_Node_Expression::generate(__ast)
            );

        }

        else { // Default constructor

            Ast_Node_Constructor_Call::set_variable_declaration_constructor(
                __ast,
                0
            );

        }

        delete __ast->open_nodes->remove(
            __ast->open_nodes->count
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
    { delete representive_type; delete parameters_type; body->~Ast_Node_Code_Block(); free(body); name_space->~Name_Space(); free(name_space); if (this_token_name) this_token_name->~Token(); free(this_token_name); delete parameters; }

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(Type_Information* __return_type, Token* __function_token_name, Name_Space* __name_space, bool __is_static)  
    : Ast_Node(AST_NODE_FUNCTION_DECLARATION, __return_type), function_token_name(__function_token_name), name_space(__name_space), is_static(__is_static), body_defined(0), destructor(0), this_token_name(0), body_position(-1), forward_declaration(0) { 

        body = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));  new (body) Ast_Node_Code_Block(0, __name_space); 

        parameters_type = new utils::Linked_List <Type_Information*>(0);

        parameters = new utils::Linked_List <Ast_Node*>();

    }

void parser::Ast_Node_Function_Declaration::set_this_variable(Ast* __ast) {

    // std::cout << "Setting this variable" << std::endl;

    this_token_name = (Token*) malloc(sizeof(Token));

    new (this_token_name) Token(
        IDENTIFIER,
        utils::get_string_copy(
            (char*) "this"
        )
    );

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

    new (_variable_declaration_node) Ast_Node_Variable_Declaration(
        parameters_type->first->object,
        this_token_name, 0
    );

    parameters->add(
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

    parameters->join(_parameters);

    for (int _ = 0; _ < _parameters->count; _++) 

        if (_parameters->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION) 

            parameters_type->add(
                _parameters->operator[](_)->representive_type
            );

    delete _parameters;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast* __ast, bool __constructor) {

    __ast->print("--> Ast Node Function Declaration <--");

    Name_Space* _name_space = get_name_space_by_path(__ast);

    if (_name_space) __ast->add_to_chain(_name_space);

    __ast->tokens_position += !__constructor;

    if (__ast->get_token(0)->id != IDENTIFIER) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

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
        Type_Information::generate_primitive_type(__ast, PRIMITIVE_TYPE_VOID), 
        _function_token_name, _function_name_space, 0
    );

    _function_declaration_node->destructor = !__constructor;

    __ast->open_nodes->add(
        _function_declaration_node, 0
    );
        
    _function_declaration_node->set_this_variable(__ast);

    _function_declaration_node->set_parameters(__ast);

    if (
        Ast_Node_Function_Declaration* _previous_function_declaration_node = 
            __ast->name_space_chain->last->object->declaration_tracker->get_function_declaration(
                _function_token_name->identifier, _function_declaration_node->parameters_type, 0, !__constructor
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

    __ast->add_to_chain(
        _function_name_space
    );

    __ast->open_nodes->add(
        _function_declaration_node->body, 0
    );

    __ast->tokens_position++;

    switch (__ast->get_token(-1)->id)
    {
    case OPEN_BRACES: _function_declaration_node->body_defined = 1; _function_declaration_node->body->set_code(__ast); break;
    case END_INSTRUCTION: break;
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(-1)); break;
    }

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

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(Ast* __ast) {

    __ast->print("--> Ast Node Function Declaration <--");

    bool _is_struct_body_function = 0;
    Token* _static_token = 0;

    if (__ast->get_token(0)->id == STATIC) 
        { _static_token = __ast->get_token(0); __ast->tokens_position++; }

    Type_Information* _return_type = Type_Information::generate(__ast, 1);

    Name_Space* _name_space = get_name_space_by_path(__ast);

    if (_name_space) __ast->add_to_chain(_name_space);

    _is_struct_body_function = __ast->name_space_chain->last->object->type == NAME_SPACE_TYPE_STRUCT_BODY;

    if (!_is_struct_body_function && _static_token) 
        throw Unexpected_Token_Ast(__ast->code_information, _static_token);

    if (__ast->get_token(0)->id != IDENTIFIER)

        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

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
        _return_type, _function_token_name, _function_name_space, !_is_struct_body_function ? 1 : (bool) _static_token
    );

    __ast->open_nodes->add(
        _function_declaration_node, 0
    );

    if (__ast->name_space_chain->last->object->type == NAME_SPACE_TYPE_STRUCT_BODY && !_static_token)
        
        _function_declaration_node->set_this_variable(__ast);

    _function_declaration_node->set_parameters(__ast);

    Ast_Node_Function_Declaration* _previous_function_declaration_node = 
            __ast->name_space_chain->last->object->declaration_tracker->get_function_declaration(
                _function_token_name->identifier, _function_declaration_node->parameters_type, _static_token, 0
            );

    if (
        _previous_function_declaration_node && _previous_function_declaration_node->body_defined && __ast->get_token(0)->id != END_INSTRUCTION
    )
        throw Redefinition_Function_Declaration_Ast(
            __ast->code_information,
            _previous_function_declaration_node->function_token_name,
            _function_token_name
        );

    else if (
        _previous_function_declaration_node && __ast->get_token(0)->id != END_INSTRUCTION
    )

        _previous_function_declaration_node->forward_declaration = 
            _function_declaration_node;

    get_current_declaration_tracker(__ast)->function_declarations->add(
        _function_declaration_node
    );

    __ast->add_to_chain(
        _function_name_space
    );

    __ast->open_nodes->add(
        _function_declaration_node->body, 0
    );

    __ast->tokens_position++;

    switch (__ast->get_token(-1)->id)
    {
    case OPEN_BRACES: _function_declaration_node->body_defined = 1; _function_declaration_node->body->set_code(__ast); break;
    case END_INSTRUCTION: break;
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(-1)); break;
    }

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


parser::Ast_Node_Expression::~Ast_Node_Expression() { 
        
    delete values; delete token_ids; 
    
    if (expression_instructions) expression_instructions->~Ast_Node(); free(expression_instructions); 
                  
}

parser::Ast_Node_Expression::Ast_Node_Expression() : Ast_Node(AST_NODE_EXPRESSION, 0), expression_instructions(0), destroy_expression_instructions(1)
    { values = new utils::Linked_List <Ast_Node*>(0); token_ids = new utils::Linked_List <Token*>(); token_ids->destroy_content = 0; }

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* __value, Token* __token_id) : Ast_Node(AST_NODE_EXPRESSION, 0), expression_instructions(0), destroy_expression_instructions(1)
    { values = new utils::Linked_List <Ast_Node*>(0); token_ids = new utils::Linked_List <Token*>(); token_ids->destroy_content = 0; values->add(__value); token_ids->add(__token_id); }

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node* __value, Ast_Node* __value_2, Token* __token_id) : Ast_Node(AST_NODE_EXPRESSION, 0), expression_instructions(0), destroy_expression_instructions(1)
    { values = new utils::Linked_List <Ast_Node*>(0); token_ids = new utils::Linked_List <Token*>(); token_ids->destroy_content = 0; values->add(__value); values->add(__value_2); token_ids->add(__token_id); }

void parser::Ast_Node_Expression::set(Ast* __ast) {

    Ast_Node* _value, *_value_2;
    Token* _token_id;

    do {

        _value = get_value(__ast);

        if (__ast->get_token(0)->id == POINTER || __ast->get_token(0)->id == ADDRESS)

            __ast->get_token(0)->id = 
                __ast->get_token(0)->id == POINTER ? FUNCTION_OPERATOR_MULTIPLICATION : FUNCTION_OPERATOR_BITWISE_AND;
    
    get_token:

        if (__ast->get_token(0)->id == OPEN_BRACKET) __ast->get_token(0)->id = FUNCTION_OPERATOR_BRACKET;

        _token_id = is_function_operator(__ast->get_token(0)->id) ? __ast->get_token(0) : 0;

        if (_token_id && is_single_parameters_function_operation(_token_id->id)) {

            Ast_Node_Expression* _expression_single_parameter = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

            new (_expression_single_parameter) Ast_Node_Expression(
                _value, _token_id
            );

            _expression_single_parameter->set_representive_type(__ast);

            _value = _expression_single_parameter;

            __ast->tokens_position++;

            try { get_node_type(__ast); }

            catch(...) { goto get_token; }

            if (get_node_type(__ast) == AST_NODE_ACCESSING) _value = Ast_Node_Accessing::generate(__ast, _value);

            goto get_token;

        }

        if (_token_id && _token_id->id == FUNCTION_OPERATOR_BRACKET) {

            __ast->tokens_position++;

            _value_2 = _value; //Ast_Node_Expression::generate(__ast);

            _value = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

            new (_value) Ast_Node_Expression(
                _value_2, Ast_Node_Expression::generate(__ast), _token_id
            );

            ( (Ast_Node_Expression*) _value)->set_representive_type(__ast);

            if (__ast->get_token(0)->id != CLOSE_BRACKET) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), CLOSE_BRACKET);

            __ast->tokens_position++;

            try { get_node_type(__ast); }

            catch(...) { goto get_token; }

            if (get_node_type(__ast) == AST_NODE_ACCESSING) _value = Ast_Node_Accessing::generate(__ast, _value);

            goto get_token;

        }

        values->add(_value); token_ids->add(_token_id);

        __ast->tokens_position++;

    } while(_token_id);

    delete token_ids->remove(
        token_ids->count
    );

    __ast->tokens_position--;

}

void parser::Ast_Node_Expression::set_representive_type(Ast* __ast) {

    utils::Linked_List <Ast_Node*>* _values = values->get_copy(0);
    utils::Linked_List <Token*>* _token_ids = token_ids->get_copy(0);

    int _current_priority = 0;


    Ast_Node_Function_Declaration* _token_function_declaration;
    char* _token_function_name;

    while ( _token_ids->count ) {

        for (int _ = 0; _ < _token_ids->count; _++) {

            if (
                get_operation_priority(
                    _token_ids->operator[](_)->id
                ) == _current_priority
            ) {

                Ast_Node_Function_Call* _function_call_node = (Ast_Node_Function_Call*) malloc(sizeof(Ast_Node_Function_Call));

                new (_function_call_node) Ast_Node_Function_Call(
                    0
                );


                // First Parameter // 

                // Get Address of first parameter for function execution
                Ast_Node_Pointer_Operation* _pointer_operation_node = 
                    (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

                new (_pointer_operation_node) Ast_Node_Pointer_Operation(
                    0, 1
                );

                _pointer_operation_node->value = 
                    _values->operator[](_);

                _pointer_operation_node->set_representive_value(__ast);


                Ast_Node_Expression* _first_paremeter_expression = 
                    (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

                new (_first_paremeter_expression) Ast_Node_Expression();

                _first_paremeter_expression->values->add(
                    _pointer_operation_node
                );

                _first_paremeter_expression->set_representive_type(__ast);

                _function_call_node->parameters->add(
                    _first_paremeter_expression
                );

                // //

                // Second Parameter //

                if ( _values->count > 1) { // Can be a expression with only one parameter ( eg. "!" )

                    Ast_Node_Expression* _second_paremeter_expression = 
                        (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

                    new (_second_paremeter_expression) Ast_Node_Expression();

                    _second_paremeter_expression->values->add(
                        _values->operator[](_ + 1)
                    );

                    _second_paremeter_expression->set_representive_type(__ast);

                    _function_call_node->parameters->add(
                        _second_paremeter_expression
                    );
                }

                // //

                // Third Parameter //

                if ( _values->operator[](_)->representive_type->pointer_level ) { // Third parameter is only needed if first parameter is pointer

                    Ast_Node_Expression* _third_paremeter_expression = 
                        (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

                    new (_third_paremeter_expression) Ast_Node_Expression();

                    _values->operator[](_)->representive_type->pointer_level--;

                    char* _implicit_value_str_mode;

                    asprintf(
                        &_implicit_value_str_mode, 
                        "%i",
                        _values->operator[](_)->representive_type->get_size()
                    );

                    _values->operator[](_)->representive_type->pointer_level++;


                    Token* _token_for_implicit_value = (Token*) malloc(sizeof(Token));

                    new (_token_for_implicit_value) Token(
                        IMPLICIT_VALUE_INT, _implicit_value_str_mode
                    );

                    Ast_Node_Implicit_Value* _implicit_value_node = (Ast_Node_Implicit_Value*) malloc(sizeof(Ast_Node_Implicit_Value));

                    new (_implicit_value_node) Ast_Node_Implicit_Value(
                        Type_Information::generate_implicit_value(__ast, IMPLICIT_VALUE_INT),
                        __ast->add_implicit_value(
                            _token_for_implicit_value
                        )
                    );

                    _implicit_value_node->remove_token = _token_for_implicit_value;

                    _third_paremeter_expression->values->add(
                        _implicit_value_node
                    );

                    _third_paremeter_expression->set_representive_type(__ast);

                    _function_call_node->parameters->add(
                        _third_paremeter_expression
                    );

                }

                _token_function_name = 
                    built_ins::get_struct_function_name_of_operation_id(_token_ids->operator[](_)->id);

                if (!_token_function_name) throw Ordinary_Exception_Ast("No Function Defined for given Toke Id");

                utils::Linked_List <Type_Information*>* _function_call_parameters_type =
                    _function_call_node->get_parameters_type();

                __ast->add_to_chain(
                    _values->operator[](_)->representive_type->pointer_level ?
                        __ast->name_space_control->get_primitive_type_name_space(
                            __ast, PRIMITIVE_TYPE_POINTER
                        ) :
                        _values->operator[](_)->representive_type->declaration->body->name_space
                );

                _token_function_declaration = 
                    get_function_declaration(
                        __ast,
                        _token_function_name,
                        _function_call_parameters_type,
                        0, 0
                    );

                delete _function_call_parameters_type;

                if (!_token_function_declaration) 
                    throw Undefined_Function_Declaration_Ast(__ast->code_information, _token_ids->operator[](_), _token_ids->operator[](_)->position_information.column);

                __ast->pop_from_chain();

                _function_call_node->representive_type = _token_function_declaration->representive_type;
                _function_call_node->function_declaration = _token_function_declaration;

                // For a pointer operation this if converts into the correct type 
                // Dependent the type of pointer it is
                if ( _values->operator[](_)->representive_type->pointer_level) {

                    // Type conversion for type
                    Ast_Node_Type_Conversion* _type_conversion_node;

                    // Return the pointer type (eg. in [] operations )

                    if ( ! strcmp(_token_function_name, FUNCTION_OPERATOR_BRACKET_FUNCTION_NAME) ) {

                        // Get down a pointer level to get the pointer type
                        _values->operator[](_)->representive_type->pointer_level --;

                        _type_conversion_node = 
                            (Ast_Node_Type_Conversion*) malloc(sizeof(Ast_Node_Type_Conversion));

                        new (_type_conversion_node) Ast_Node_Type_Conversion(
                            _values->operator[](_)->representive_type->get_copy(), _function_call_node
                        );

                        _values->operator[](_)->representive_type->pointer_level ++;                        

                    }

                    // Return a pointer of type
                    else {

                        _type_conversion_node = 
                            (Ast_Node_Type_Conversion*) malloc(sizeof(Ast_Node_Type_Conversion));

                        new (_type_conversion_node) Ast_Node_Type_Conversion(
                            _values->operator[](_)->representive_type->get_copy(), _function_call_node
                        );

                    }

                    _values->getDataLinkedList(_)->object = _type_conversion_node;

                }

                else _values->getDataLinkedList(_)->object = _function_call_node;

                free(_token_function_name);

                

                if ( _values->count > 1) {

                    _values->getDataLinkedList(_ + 1)->destroy_content = 0;

                    delete _values->remove(_ + 1);

                }
                
                _token_ids->getDataLinkedList(_)->destroy_content = 0;

                delete _token_ids->remove(_);

                _--;

            }
        
        }

        _current_priority++;

    }

    representive_type = _values->operator[](0)->representive_type;

    expression_instructions = _values->operator[](0);

    delete _token_ids;
    delete _values;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate_single_parameter(Ast* __ast) {

    __ast->print("--> Ast Node Expression Single Parameter <--");

    Ast_Node_Expression* _expression_node = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

    new (_expression_node) Ast_Node_Expression();

    __ast->open_nodes->add(
        _expression_node, 0
    );

    if (__ast->get_token(0)->id == FUNCTION_OPERATOR_INCREMENT || __ast->get_token(0)->id == FUNCTION_OPERATOR_DECREMENT)

        __ast->get_token(0)->id = 
            __ast->get_token(0)->id == FUNCTION_OPERATOR_INCREMENT ? FUNCTION_OPERATOR_INCREMENT_LEFT : FUNCTION_OPERATOR_DECREMENT_LEFT;

    _expression_node->token_ids->add(
        __ast->get_token(0)
    );

    __ast->tokens_position++;

    _expression_node->values->add(
        get_value(__ast)
    );

    _expression_node->set_representive_type(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Expression Single Parameter End <--");

    return _expression_node;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast* __ast) {

    __ast->print("--> Ast Node Expression <--");

    Ast_Node_Expression* _expression_node = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

    new (_expression_node) Ast_Node_Expression();

    __ast->open_nodes->add(
        _expression_node, 0
    );

    _expression_node->set(__ast);

    _expression_node->set_representive_type(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Expression End <--");

    return _expression_node;

}

int parser::Ast_Node_Expression::get_operation_priority(int __operation_id) {

    if ( ( __operation_id >= FUNCTION_OPERATOR_INCREMENT && __operation_id <= FUNCTION_OPERATOR_BRACKET ) || __operation_id == FUNCTION_OPERATOR_NOT || __operation_id == FUNCTION_OPERATOR_BITWISE_NOT ) return 0;
    if (__operation_id >= FUNCTION_OPERATOR_MULTIPLICATION && __operation_id <= FUNCTION_OPERATOR_MODULOS) return 1;
    if (__operation_id >= FUNCTION_OPERATOR_PLUS && __operation_id <= FUNCTION_OPERATOR_MINUS) return 2;
    if (__operation_id >= FUNCTION_OPERATOR_BITWISE_AND && __operation_id <= FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT) return 3;
    if (__operation_id >= FUNCTION_OPERATOR_AND && __operation_id <= FUNCTION_OPERATOR_OR) return 4;
    if (__operation_id >= FUNCTION_OPERATOR_EQUAL_TO && __operation_id <= FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO) return 5;

    return 6;

}

bool parser::Ast_Node_Expression::is_function_operator(int __token_id) 
    { return __token_id >= FUNCTION_OPERATOR_PLUS && __token_id <= FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT_ASSIGN; }

parser::Ast_Node* parser::Ast_Node_Expression::get_value(Ast* __ast) {

    // std::cout << "Value type -> " << get_node_type(__ast) << std::endl;

    switch (get_node_type(__ast))
    {
    case AST_NODE_CAST: return Ast_Node_Cast::generate(__ast); break;
    case AST_NODE_VARIABLE: return Ast_Node_Variable::generate(__ast); break;
    case AST_NODE_PARENTHESIS: return Ast_Node_Parenthesis::generate(__ast); break;
    case AST_NODE_FUNCTION_CALL: return Ast_Node_Function_Call::generate(__ast); break;
    case AST_NODE_IMPLICIT_VALUE: return Ast_Node_Implicit_Value::generate(__ast); break;
    case AST_NODE_TYPE_CONVERSION: return Ast_Node_Type_Conversion::generate(__ast); break;
    case AST_NODE_POINTER_OPERATION: return Ast_Node_Pointer_Operation::generate(__ast); break;
    case AST_NODE_EXPRESSION: return Ast_Node_Expression::generate_single_parameter(__ast); break; 
    case AST_BITWISE_NOT: case AST_NODE_CONSTRUCTOR_CALL: return Ast_Node_Constructor_Call::generate(__ast); break;
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
    }

    return 0;

}



parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(Ast_Node_Variable_Declaration* __variable_declaration, Token* __variable_token_name) 
    : Ast_Node(AST_NODE_VARIABLE, __variable_declaration->representive_type), variable_declaration(__variable_declaration), variable_token_name(__variable_token_name) {}

parser::Ast_Node* parser::Ast_Node_Variable::generate(Ast* __ast) {
    
    __ast->print("--> Ast Node Variable <--");

    int _backup_state = __ast->get_token(0)->position_information.column;

    Name_Space* _name_space;

    if (_name_space = get_name_space_by_path(__ast)) __ast->add_to_chain(_name_space);

    if (__ast->get_token(0)->id != IDENTIFIER) 
        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

    Token* _variable_token_name = __ast->get_token(0);
    __ast->tokens_position++;

    Ast_Node_Variable_Declaration* _variable_declaration_node =
        get_variable_declaration(__ast, _variable_token_name->identifier);

    if (!_variable_declaration_node) 

        throw Undefined_Variable_Declaration_Ast(__ast->code_information, _variable_token_name, _backup_state);

    Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));

    new (_variable_node) Ast_Node_Variable(
        _variable_declaration_node, _variable_token_name
    );

    if (_name_space) __ast->pop_from_chain();

    __ast->print("--> Ast Node Variable End <--");
    
    try { get_node_type(__ast); }

    catch(...) { return _variable_node; }

    return get_node_type(__ast) == AST_NODE_ACCESSING ? (Ast_Node*) Ast_Node_Accessing::generate(__ast, _variable_node) : (Ast_Node*) _variable_node;

}



parser::Ast_Node_Function_Call::~Ast_Node_Function_Call() { delete parameters; }

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(Token* __function_token_name) 
    : Ast_Node(AST_NODE_FUNCTION_CALL, 0), function_token_name(__function_token_name), expression_helper(0)
        { parameters = new utils::Linked_List <Ast_Node_Expression*>(); }

void parser::Ast_Node_Function_Call::set_parameters(Ast* __ast) {

    __ast->print("--> Function Call Parameters <--");

    __ast->tokens_position++;

    __ast->open_nodes->add(0, 0);

    while(__ast->get_token(0)->id != CLOSE_PARENTHESIS) {

        parameters->add(
            Ast_Node_Expression::generate(
                __ast
            )
        );

        switch (__ast->get_token(0)->id)
        {
        case COMMA: __ast->tokens_position++; break;
        case CLOSE_PARENTHESIS: break;
        default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

    __ast->tokens_position++;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Function Call Parameters End <--");

}

void parser::Ast_Node_Function_Call::set_function_declaration(Ast* __ast, Name_Space* __name_space, int __backup_state, bool __is_static, bool __is_destructor) {

    __ast->print("--> Function Call Set Function Declaration <--");

    if (__name_space) __ast->add_to_chain(__name_space);

    utils::Linked_List <Type_Information*>* _parameters_type = get_parameters_type();

    function_declaration = 
        get_function_declaration(
            __ast, 
            function_token_name->identifier, 
            _parameters_type,
            __is_static,
            __is_destructor
        );
    
    if (!function_declaration) 
        { delete _parameters_type; throw Undefined_Function_Declaration_Ast(__ast->code_information, __ast->get_token(-1), __backup_state);}

    representive_type = function_declaration->representive_type;

    delete _parameters_type;

    if (__name_space) __ast->pop_from_chain();

    __ast->print("--> Function Call Set Function Declaration End <--");

}

utils::Linked_List <parser::Type_Information*>* parser::Ast_Node_Function_Call::get_parameters_type() {

    utils::Linked_List <Type_Information*>* _parameters_type = 
        new utils::Linked_List <Type_Information*>(0);

    for (int _ = 0; _ < parameters->count; _++) 
        _parameters_type->add(parameters->operator[](_)->representive_type);

    return _parameters_type;

}


parser::Ast_Node* parser::Ast_Node_Function_Call::generate_accessing_function_call(Ast* __ast, Ast_Node_Expression* __first_parameter) {

    __ast->print("--> Ast Node Function Call Accessing <--");

    Name_Space* _backup_name_space = 
        __ast->name_space_chain->last->object;

    __ast->pop_from_chain();

    int _backup_state = __ast->get_token(0)->position_information.column;

    bool _is_destructor = __ast->get_token(0)->id == FUNCTION_OPERATOR_BITWISE_NOT;

    if (_is_destructor) __ast->tokens_position++;

    if (__ast->get_token(0)->id != IDENTIFIER) 

        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

    Token* _function_token_name = __ast->get_token(0);
    __ast->tokens_position++;

    Ast_Node_Function_Call* _function_call_node = (Ast_Node_Function_Call*) malloc(sizeof(Ast_Node_Function_Call));

    new (_function_call_node) Ast_Node_Function_Call(
        _function_token_name
    );

    __ast->open_nodes->add(
        _function_call_node, 0
    );

    _function_call_node->set_parameters(__ast);

    _function_call_node->parameters->insert(
        __first_parameter, 0
    );

    __ast->add_to_chain(
        _backup_name_space
    );

    _function_call_node->set_function_declaration(__ast, 0, _backup_state, 0, _is_destructor); 

    // delete _function_call_node->parameters->remove(0);

    // _function_call_node->parameters->insert(0, 0);

    __ast->print("--> Ast Node Function Call Accessing End <--");

    if (get_node_type(__ast) == AST_NODE_ACCESSING)

        return Ast_Node_Accessing::generate(__ast, _function_call_node);

    return _function_call_node;

}

parser::Ast_Node* parser::Ast_Node_Function_Call::generate(Ast* __ast) {

    __ast->print("--> Ast Node Function Call <--");

    int _backup_state = __ast->get_token(0)->position_information.column;

    Name_Space* _name_space = get_name_space_by_path(__ast);

    if (__ast->get_token(0)->id != IDENTIFIER) 

        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), IDENTIFIER);

    Token* _function_token_name = __ast->get_token(0);
    __ast->tokens_position++;

    Ast_Node_Function_Call* _function_call_node = (Ast_Node_Function_Call*) malloc(sizeof(Ast_Node_Function_Call));

    new (_function_call_node) Ast_Node_Function_Call(
        _function_token_name
    );

    __ast->open_nodes->add(
        _function_call_node, 0
    );

    _function_call_node->set_parameters(__ast);

    _function_call_node->set_function_declaration(__ast, _name_space, _backup_state, 1, 0);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Function Call End <--");

    try { get_node_type(__ast); }

    catch(...) { return _function_call_node; }

    return get_node_type(__ast) == AST_NODE_ACCESSING ? (Ast_Node*) Ast_Node_Accessing::generate(__ast, _function_call_node) : (Ast_Node*) _function_call_node;

}



parser::Ast_Node_Implicit_Value::~Ast_Node_Implicit_Value() { delete representive_type; if (remove_token) remove_token->~Token(); free(remove_token); }

parser::Ast_Node_Implicit_Value::Ast_Node_Implicit_Value(Type_Information* __type, int __implicit_value_position) 
    : Ast_Node(AST_NODE_IMPLICIT_VALUE, __type), implicit_value_position(__implicit_value_position), remove_token(0) {}

parser::Ast_Node* parser::Ast_Node_Implicit_Value::generate(Ast* __ast) {

    __ast->print("--> Ast Node Implicit Value <--");

    Ast_Node_Implicit_Value* _implicit_value_node = (Ast_Node_Implicit_Value*) malloc(sizeof(Ast_Node_Implicit_Value));

    new (_implicit_value_node) Ast_Node_Implicit_Value(
        Type_Information::generate_implicit_value(__ast, __ast->get_token(0)->id),
        __ast->add_implicit_value(
            __ast->get_token(0)
        )
    );

    __ast->tokens_position++;

    try { get_node_type(__ast); }

    catch(...) { return _implicit_value_node; }

    return get_node_type(__ast) == AST_NODE_ACCESSING ? (Ast_Node*) Ast_Node_Accessing::generate(__ast, _implicit_value_node) : (Ast_Node*) _implicit_value_node;

    
    return _implicit_value_node;

}



parser::Ast_Node_Pointer_Operation::~Ast_Node_Pointer_Operation() 
    { delete representive_type; if (pointer_operations) delete pointer_operations; if (destroy_value) { value->~Ast_Node(); free(value); } }

parser::Ast_Node_Pointer_Operation::Ast_Node_Pointer_Operation(utils::Linked_List <Token*>* __pointer_operations, int __pointer_level) 
    : Ast_Node(AST_NODE_POINTER_OPERATION, 0), pointer_operations(__pointer_operations), pointer_level(__pointer_level), destroy_value(1) {}

void parser::Ast_Node_Pointer_Operation::set_value(Ast* __ast) { value = get_value(__ast); }

void parser::Ast_Node_Pointer_Operation::set_representive_value(Ast* __ast) {

    representive_type = value->representive_type->get_copy();

    representive_type->pointer_level += pointer_level;

    if (representive_type->pointer_level < 0) 

        throw Pointer_Operations_Below_0_Ast(__ast->code_information, pointer_operations);
    
}

parser::Ast_Node_Pointer_Operation* parser::Ast_Node_Pointer_Operation::generate(Ast* __ast) {

    __ast->print("--> Ast Node Pointer Operation <--");

    utils::Linked_List <Token*>* _pointer_operations = 
        new utils::Linked_List <Token*>(0);
    int _pointer_level = 0;

    while (
        __ast->get_token(0)->id == POINTER || __ast->get_token(0)->id == ADDRESS
    ) { _pointer_operations->add(__ast->get_token(0)); _pointer_level += __ast->get_token(0)->id == POINTER ? -1 : 1; __ast->tokens_position++; }
    
    Ast_Node_Pointer_Operation* _pointer_operation_node = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

    new (_pointer_operation_node) Ast_Node_Pointer_Operation(
        _pointer_operations, _pointer_level
    );

    __ast->open_nodes->add(
        _pointer_operation_node, 0
    );

    _pointer_operation_node->set_value(__ast);

    _pointer_operation_node->set_representive_value(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Pointer Operation End <--");

    return _pointer_operation_node;

}



parser::Ast_Node_Parenthesis::~Ast_Node_Parenthesis() { expression->~Ast_Node_Expression(); free(expression); } 

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis() : Ast_Node(AST_NODE_PARENTHESIS, 0) {}

void parser::Ast_Node_Parenthesis::set_expression(Ast* __ast) { expression = Ast_Node_Expression::generate(__ast); }

void parser::Ast_Node_Parenthesis::set_representive_type() { representive_type = expression->representive_type; }

parser::Ast_Node* parser::Ast_Node_Parenthesis::generate(Ast* __ast) {

    __ast->print("--> Ast Node Parenthesis <--");

    __ast->tokens_position++;
    
    Ast_Node_Parenthesis* _parenthesis_node = (Ast_Node_Parenthesis*) malloc(sizeof(Ast_Node_Parenthesis));

    new (_parenthesis_node) Ast_Node_Parenthesis();

    __ast->open_nodes->add(
        _parenthesis_node, 0
    );

    _parenthesis_node->set_expression(__ast);

    if (__ast->get_token(0)->id != CLOSE_PARENTHESIS) 
        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), CLOSE_PARENTHESIS);

    __ast->tokens_position++;

    _parenthesis_node->set_representive_type();

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Parenthesis End <--");

    try { get_node_type(__ast); }

    catch(...) { return _parenthesis_node; }

    return get_node_type(__ast) == AST_NODE_ACCESSING ? (Ast_Node*) Ast_Node_Accessing::generate(__ast, _parenthesis_node) : (Ast_Node*) _parenthesis_node;

    return _parenthesis_node;

}



parser::Ast_Node_Accessing::~Ast_Node_Accessing() { accessing->~Ast_Node(); free(accessing); value->~Ast_Node(); free(value); }

parser::Ast_Node_Accessing::Ast_Node_Accessing(Ast_Node* __value, bool __is_pointer_accessing) 
    : Ast_Node(AST_NODE_ACCESSING, 0), value(__value), is_pointer_accessing(__is_pointer_accessing) {}

void parser::Ast_Node_Accessing::set_representive_type(Ast* __ast) {

    Ast_Node* _accessing_value = accessing;

    while(_accessing_value->node_type == AST_NODE_ACCESSING) 

        _accessing_value = (
            (Ast_Node_Accessing*) _accessing_value
        )->accessing;

    representive_type = 
        _accessing_value->representive_type;

}

void parser::Ast_Node_Accessing::set_accessing(Ast* __ast) {

    // std::cout << "Node Type -> " << get_node_type(__ast) << std::endl;

    __ast->add_to_chain(
        !is_pointer_accessing && value->representive_type->pointer_level ? 
            __ast->name_space_control->get_primitive_type_name_space(__ast, PRIMITIVE_TYPE_POINTER)
            : value->representive_type->declaration->body->name_space
    );

    switch (int _node_type = get_node_type(__ast))
    {
    case AST_NODE_VARIABLE: accessing = Ast_Node_Variable::generate(__ast); break; 
    case AST_NODE_FUNCTION_CALL: case AST_NODE_CONSTRUCTOR_CALL: case AST_BITWISE_NOT:

        {

            // if (_node_type == AST_BITWISE_NOT) __ast->tokens_position++;

            Ast_Node_Pointer_Operation* _pointer_operation_node = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

            new (_pointer_operation_node) Ast_Node_Pointer_Operation(
                0, 
                is_pointer_accessing ? 0 : 1
            );

            _pointer_operation_node->value = value;

            _pointer_operation_node->destroy_value = 0;

            _pointer_operation_node->set_representive_value(__ast);
    
            Ast_Node_Expression* _first_parameter_expression = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

            new (_first_parameter_expression) Ast_Node_Expression();

            _first_parameter_expression->values->add(
                _pointer_operation_node
            );

            _first_parameter_expression->set_representive_type(__ast);

            accessing = Ast_Node_Function_Call::generate_accessing_function_call(
                __ast, _first_parameter_expression
            ); 
            

            
            break;

        }
        
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
    }

    __ast->pop_from_chain(); 
}

parser::Ast_Node_Accessing* parser::Ast_Node_Accessing::generate(Ast* __ast, Ast_Node* __value) {

    __ast->print("--> Ast Node Accessing <--");

    bool _is_pointer_accessing = __ast->get_token(0)->id == ACCESSING_POINTER;

    if (_is_pointer_accessing && !__value->representive_type->pointer_level)

        throw Unmatched_Accessing_Operation_Ast(__ast->code_information, __ast->get_token(0)); 

    __ast->tokens_position++;

    Ast_Node_Accessing* _accessing_node = (Ast_Node_Accessing*) malloc(sizeof(Ast_Node_Accessing));

    new (_accessing_node) Ast_Node_Accessing(
        __value, _is_pointer_accessing
    );

    __ast->open_nodes->add(
        _accessing_node, 0
    );

    _accessing_node->set_accessing(__ast);

    _accessing_node->set_representive_type(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Accessing End <--");

    return _accessing_node;

}



parser::Ast_Node_Byte_Code::~Ast_Node_Byte_Code() { code->~Ast_Node(); free(code); argument->~Ast_Node(); free(argument); }

parser::Ast_Node_Byte_Code::Ast_Node_Byte_Code() : Ast_Node(AST_NODE_BYTE_CODE, 0) {}

parser::Ast_Node_Byte_Code* parser::Ast_Node_Byte_Code::generate(Ast* __ast) {

    __ast->print("--> Ast Node Byte Code <--");

    __ast->tokens_position++;

    Ast_Node_Byte_Code* _byte_code_node = (Ast_Node_Byte_Code*) malloc(sizeof(Ast_Node_Byte_Code));

    new (_byte_code_node) Ast_Node_Byte_Code();

    __ast->open_nodes->add(
        _byte_code_node, 0
    );

    _byte_code_node->code = Ast_Node_Expression::generate(__ast);

    if (__ast->get_token(0)->id != COMMA) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), COMMA);

    __ast->tokens_position++;

    _byte_code_node->argument = Ast_Node_Expression::generate(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Byte Code End <--");

    return _byte_code_node;

}



parser::Ast_Node_Constructor_Call::~Ast_Node_Constructor_Call() { 

    constructor->function_token_name->~Token(); free(constructor->function_token_name); 

    constructor->~Ast_Node_Function_Call(); free(constructor);

    if (this_variable && delete_this_variable) { this_variable->~Ast_Node_Variable_Declaration(); free(this_variable); }

    delete representive_type;

}

parser::Ast_Node_Constructor_Call::Ast_Node_Constructor_Call(Type_Information* __representive_type, char* __search_name)
    : Ast_Node(AST_NODE_CONSTRUCTOR_CALL, __representive_type), this_variable(0), search_name(__search_name), delete_this_variable(0) {
        
        Token* _token_function_name = (Token*) malloc(sizeof(Token));

        new (_token_function_name) Token(
            IDENTIFIER, 
            utils::get_string_copy(
                search_name ? search_name :
                __representive_type->declaration->struct_token_name->identifier
            )
        );

        constructor = (Ast_Node_Function_Call*) malloc(sizeof(Ast_Node_Function_Call)); 
        
        new (constructor) Ast_Node_Function_Call(_token_function_name); 
        
}

void parser::Ast_Node_Constructor_Call::set_add_this_variable(Ast* __ast) {

    this_variable = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (this_variable) Ast_Node_Variable_Declaration(
        representive_type->get_copy(), 0, 0
    );

    // this_variable->representive_type->pointer_level++;

    //

    Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));

    new (_variable_node) Ast_Node_Variable(
        this_variable, 0
    );

    Ast_Node_Pointer_Operation* _pointer_operation_node = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

    new (_pointer_operation_node) Ast_Node_Pointer_Operation(
        0, 1
    );

    _pointer_operation_node->value = _variable_node;

    _pointer_operation_node->set_representive_value(__ast);

    Ast_Node_Expression* _expression_node = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

    new (_expression_node) Ast_Node_Expression();

    _expression_node->values->add(
        _pointer_operation_node
    );

    _expression_node->set_representive_type(__ast);

    constructor->parameters->insert(
        _expression_node, 0
    );

}

void parser::Ast_Node_Constructor_Call::set_add_this_variable(Ast* __ast, Ast_Node_Variable_Declaration* __variable_declaration) {

    this_variable = __variable_declaration;

    Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));

    new (_variable_node) Ast_Node_Variable(
        __variable_declaration, 0
    );

    Ast_Node_Pointer_Operation* _pointer_operation_node = 
        (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

    new (_pointer_operation_node) Ast_Node_Pointer_Operation(
        0, 1
    );

    _pointer_operation_node->value = _variable_node;

    _pointer_operation_node->set_representive_value(__ast);

    Ast_Node_Expression* _expression_node = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

    new (_expression_node) Ast_Node_Expression();

    _expression_node->values->add(
        _pointer_operation_node
    );

    _expression_node->set_representive_type(__ast);

    constructor->parameters->insert(
        _expression_node, 0
    );

}

parser::Ast_Node_Constructor_Call* parser::Ast_Node_Constructor_Call::generate(Ast* __ast) {

    __ast->print("--> Ast Node Constructor Call <--");

    int _backup_state = __ast->get_token(0)->position_information.column;
    int _backup_token_state = __ast->tokens_position;

    Type_Information* _struct_declaration_type;
    char* _search_name = 0;

    if (Name_Space* _constructor_name_space = get_name_space_by_path(__ast)) {

        __ast->add_to_chain(
            _constructor_name_space
        );

        _struct_declaration_type = Type_Information::generate(__ast, 1);

    }

    else  {

        __ast->tokens_position = _backup_token_state;

        _struct_declaration_type = Type_Information::generate(__ast, 1);

        __ast->add_to_chain(
            _struct_declaration_type->declaration->body->name_space
        );

    }

    if (_struct_declaration_type->pointer_level) {

        __ast->pop_from_chain();

        __ast->add_to_chain(
            __ast->name_space_control->get_primitive_type_name_space(__ast, PRIMITIVE_TYPE_POINTER)
        );

        _search_name = PRIMITIVE_TYPE_POINTER_NAME;

    }


    Ast_Node_Constructor_Call* _constructor_call_node = (Ast_Node_Constructor_Call*) malloc(sizeof(Ast_Node_Constructor_Call));

    new (_constructor_call_node) Ast_Node_Constructor_Call(
        _struct_declaration_type, _search_name
    );

    __ast->open_nodes->add(
        _constructor_call_node, 0
    );

    _constructor_call_node->constructor->set_parameters(__ast);

    _constructor_call_node->set_add_this_variable(__ast);

    _constructor_call_node->constructor->set_function_declaration(__ast, 0, _backup_state, 0, 0);

    // delete _constructor_call_node->constructor->parameters->remove(
    //     0
    // );

    // _constructor_call_node->constructor->parameters->insert(0,0);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node Constructor Call End <--");

    return _constructor_call_node;

}

parser::Ast_Node_Constructor_Call* parser::Ast_Node_Constructor_Call::generate(Ast* __ast, Type_Information* __struct_declaration_type, Ast_Node* __value) {

    __ast->print("--> Ast Node Constructor Call - Cast <--");

    int _backup_state = __ast->get_token(0)->position_information.column;
    char* _search_name = 0;

    if (__struct_declaration_type->pointer_level) {

        __ast->add_to_chain(
            __ast->name_space_control->get_primitive_type_name_space(__ast, PRIMITIVE_TYPE_POINTER)
        );

        _search_name = PRIMITIVE_TYPE_POINTER_NAME;

    }

    else {

        __ast->add_to_chain(
            __struct_declaration_type->declaration->body->name_space
        );

    }

    Ast_Node_Constructor_Call* _constructor_call_node = (Ast_Node_Constructor_Call*) malloc(sizeof(Ast_Node_Constructor_Call));

    new (_constructor_call_node) Ast_Node_Constructor_Call(
        __struct_declaration_type, _search_name
    );

    __ast->open_nodes->add(
        _constructor_call_node, 0
    );

    Ast_Node_Expression* _expression = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

    new (_expression) Ast_Node_Expression();

    _expression->values->add(
        __value, 0 
    );

    _expression->set_representive_type(__ast);

    _constructor_call_node->constructor->parameters->add(
        _expression
    );

    _constructor_call_node->set_add_this_variable(__ast);

    _constructor_call_node->constructor->set_function_declaration(__ast, 0, _backup_state, 0, 0);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node Constructor Call - Cast End <--");

    return _constructor_call_node;

}

void parser::Ast_Node_Constructor_Call::set_variable_declaration_constructor(Ast* __ast, Ast_Node_Expression* __expression) {

    __ast->print("--> Ast Node Constructor Call - Variable Declaration <--");

    int _backup_state = __ast->get_token(0)->position_information.column;
    char* _search_name = 0;


    Ast_Node_Variable_Declaration* _variable_declaration = 
        (Ast_Node_Variable_Declaration*) __ast->open_nodes->last->object;

    Type_Information* _struct_declaration_type = 
        _variable_declaration->representive_type->get_copy();

    if (_struct_declaration_type->pointer_level) {

        __ast->add_to_chain(
            __ast->name_space_control->get_primitive_type_name_space(__ast, PRIMITIVE_TYPE_POINTER)
        );

        _search_name = PRIMITIVE_TYPE_POINTER_NAME;

    }

    else {

        __ast->add_to_chain(
            _struct_declaration_type->declaration->body->name_space
        );

    }

    Ast_Node_Constructor_Call* _constructor_call_node = (Ast_Node_Constructor_Call*) malloc(sizeof(Ast_Node_Constructor_Call));

    new (_constructor_call_node) Ast_Node_Constructor_Call(
        _struct_declaration_type, _search_name
    );

    __ast->open_nodes->add(
        _constructor_call_node, 0
    );

    if (__expression) _constructor_call_node->constructor->parameters->add(__expression);

    _constructor_call_node->set_add_this_variable(__ast, _variable_declaration);

    _constructor_call_node->constructor->set_function_declaration(__ast, 0, _backup_state, 0, 0);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node Constructor Call - Variable Declaration End <--");

    // std::cout << "Node type -> " << _constructor_call_node->constructor->node_type << std::endl;

    _variable_declaration->constructor_call =
        _constructor_call_node;

}




parser::Ast_Node_Cast::~Ast_Node_Cast() 
    { constructor_call->~Ast_Node_Constructor_Call(); free(constructor_call); }

parser::Ast_Node_Cast::Ast_Node_Cast(Type_Information* __cast_to_type) 
    : Ast_Node(AST_NODE_CAST, __cast_to_type), cast_to_type(__cast_to_type) {}

void parser::Ast_Node_Cast::set_constructor(Ast* __ast, int __inicial_position) {

    constructor_call = 
        Ast_Node_Constructor_Call::generate(
            __ast, cast_to_type, value
        );

}

void parser::Ast_Node_Cast::set_value(Ast* __ast) {

    switch (get_node_type(__ast))
    {
    case AST_NODE_CAST: value = Ast_Node_Cast::generate(__ast); break;
    case AST_NODE_VARIABLE: value = Ast_Node_Variable::generate(__ast); break;
    case AST_NODE_PARENTHESIS: value = Ast_Node_Parenthesis::generate(__ast); break;
    case AST_NODE_FUNCTION_CALL: value = Ast_Node_Function_Call::generate(__ast); break;
    case AST_NODE_IMPLICIT_VALUE: value = Ast_Node_Implicit_Value::generate(__ast); break;
    case AST_NODE_POINTER_OPERATION: value = Ast_Node_Pointer_Operation::generate(__ast); break;
    case AST_NODE_EXPRESSION: value = Ast_Node_Expression::generate_single_parameter(__ast); break; 
    case AST_BITWISE_NOT: case AST_NODE_CONSTRUCTOR_CALL: value = Ast_Node_Constructor_Call::generate(__ast); break;
    default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
    }

}

parser::Ast_Node_Cast* parser::Ast_Node_Cast::generate(Ast* __ast) {

    __ast->print("--> Ast Node Cast <--");

    __ast->tokens_position++;

    Type_Information* _cast_to_type = Type_Information::generate(__ast, 1);

    __ast->tokens_position++;

    int _backup_state = __ast->get_token(0)->position_information.column;

    Ast_Node_Cast* _cast_node = (Ast_Node_Cast*) malloc(sizeof(Ast_Node_Cast));

    new (_cast_node) Ast_Node_Cast(_cast_to_type);

    __ast->open_nodes->add(_cast_node, 0);

    _cast_node->set_value(__ast);

    _cast_node->set_constructor(__ast, _backup_state);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Cast End <--");

    return _cast_node;

}



parser::Ast_Node_Return_Key_Word::~Ast_Node_Return_Key_Word() { expression->~Ast_Node_Expression(); free(expression); }

parser::Ast_Node_Return_Key_Word::Ast_Node_Return_Key_Word() : Ast_Node(AST_NODE_RETURN_KEY_WORD, 0) {}

void parser::Ast_Node_Return_Key_Word::confirm_return_type(Ast* __ast, int __inicial_position) {

    Ast_Node_Function_Declaration* _function_declaration = 0;

    for (int _  = __ast->open_nodes->count - 1; _ >= 0 && !_function_declaration; _--)

        if (
            __ast->open_nodes->operator[](_)->node_type == AST_NODE_FUNCTION_DECLARATION
        ) _function_declaration = (Ast_Node_Function_Declaration*)__ast->open_nodes->operator[](_);

    if (!_function_declaration) throw Ordinary_Exception_Ast("Internal error");

    if (
        representive_type->operator!=(
            _function_declaration->representive_type
        )
    ) throw Function_Return_And_Given_Return_Not_The_Same(__ast->code_information, __ast->get_token(-1), __inicial_position);

    function_return_type = 
        _function_declaration->representive_type;

}

void parser::Ast_Node_Return_Key_Word::set_expression(Ast* __ast) 
    { expression = Ast_Node_Expression::generate(__ast); representive_type = expression->representive_type; }

parser::Ast_Node_Return_Key_Word* parser::Ast_Node_Return_Key_Word::generate(Ast* __ast) {

    __ast->print("--> Ast Node Return Key Word <--");

    __ast->tokens_position++;

    int _inicial_position = __ast->get_token(0)->position_information.column;

    Ast_Node_Return_Key_Word* _return_key_word_node = (Ast_Node_Return_Key_Word*) malloc(sizeof(Ast_Node_Return_Key_Word));

    new (_return_key_word_node) Ast_Node_Return_Key_Word();

    __ast->open_nodes->add(
        _return_key_word_node, 0
    );

    _return_key_word_node->set_expression(__ast);

    _return_key_word_node->confirm_return_type(__ast, _inicial_position);

    std::cout << "Current token id -> " << __ast->get_token(0)->id << std::endl;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node Return Key Word End <--");

    return _return_key_word_node;

}



parser::Ast_Node_While::~Ast_Node_While() { body->~Ast_Node_Code_Block(); free(body); condition->~Ast_Node_Variable_Declaration(); free(condition); } 

parser::Ast_Node_While::Ast_Node_While(Ast_Node_Code_Block* __body) : Ast_Node(AST_NODE_WHILE, 0), body(__body) {}

void parser::Ast_Node_While::set_condition(Ast* __ast) {

    if (__ast->get_token(0)->id != OPEN_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), OPEN_PARENTHESIS);

    __ast->tokens_position++;

    condition = parser::get_condition(__ast);

    if (__ast->get_token(0)->id != CLOSE_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), CLOSE_PARENTHESIS);

    __ast->tokens_position++;

}

void parser::Ast_Node_While::set_body(Ast* __ast) { body->set_code(__ast, 1); }

parser::Ast_Node_While* parser::Ast_Node_While::generate(Ast* __ast) {

    __ast->print("--> Ast Node While <--");

    __ast->tokens_position++;

    Name_Space* _body_name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _body_code_block = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_body_code_block) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _body_name_space
    );

    Ast_Node_While* _while_node = (Ast_Node_While*) malloc(sizeof(Ast_Node_While));

    new (_while_node) Ast_Node_While(
        _body_code_block
    );

    __ast->open_nodes->add(
        _while_node, 0
    );

    __ast->open_nodes->add(
        _body_code_block, 0
    );

    __ast->add_to_chain(
        _body_name_space
    );

    _while_node->set_condition(__ast);

    _while_node->set_body(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node While End <--");

    return _while_node;

}



parser::Ast_Node_Do_While::~Ast_Node_Do_While() { body->~Ast_Node_Code_Block(); free(body); condition->~Ast_Node_Variable_Declaration(); free(condition); }

parser::Ast_Node_Do_While::Ast_Node_Do_While(Ast_Node_Code_Block* __body) : Ast_Node(AST_NODE_DO_WHILE, 0), body(__body) {}

void parser::Ast_Node_Do_While::set_condition(Ast* __ast) {

    if (__ast->get_token(0)->id != OPEN_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), OPEN_PARENTHESIS);

    __ast->tokens_position++;

    condition = parser::get_condition(__ast);

    if (__ast->get_token(0)->id != CLOSE_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), CLOSE_PARENTHESIS);

    __ast->tokens_position++;

}

void parser::Ast_Node_Do_While::set_body(Ast* __ast) { body->set_code(__ast, 1); }

parser::Ast_Node_Do_While* parser::Ast_Node_Do_While::generate(Ast* __ast) {

    __ast->print("--> Ast Node Do While <--");

    __ast->tokens_position++;

    Name_Space* _body_name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _body_code_block = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_body_code_block) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _body_name_space
    );

    Ast_Node_Do_While* _do_while_node = (Ast_Node_Do_While*) malloc(sizeof(Ast_Node_Do_While));

    new (_do_while_node) Ast_Node_Do_While(
        _body_code_block
    );

    __ast->open_nodes->add(
        _do_while_node, 0
    );

    __ast->open_nodes->add(
        _body_code_block, 0
    );

    __ast->add_to_chain(
        _body_name_space
    );

    _do_while_node->set_body(__ast);

    if (__ast->get_token(0)->id != WHILE) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), WHILE);

    __ast->tokens_position++;

    _do_while_node->set_condition(__ast);

    if (__ast->get_token(0)->id != END_INSTRUCTION) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), END_INSTRUCTION);

    __ast->tokens_position++;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );
    
    __ast->pop_from_chain();

    __ast->print("--> Ast Node Do While End <--");

    return _do_while_node;

}



parser::Ast_Node_If::~Ast_Node_If() { body->~Ast_Node_Code_Block(); free(body); condition->~Ast_Node_Variable_Declaration(); free(condition); }

parser::Ast_Node_If::Ast_Node_If(Ast_Node_Code_Block* __body) : Ast_Node(AST_NODE_IF, 0), body(__body), next_if_statement(0) {}

void parser::Ast_Node_If::set_condition(Ast* __ast) {

    if (__ast->get_token(0)->id != OPEN_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), OPEN_PARENTHESIS);

    __ast->tokens_position++;

    condition = parser::get_condition(__ast);

    if (__ast->get_token(0)->id != CLOSE_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), CLOSE_PARENTHESIS);

    __ast->tokens_position++;

}

void parser::Ast_Node_If::set_body(Ast* __ast) { body->set_code(__ast, 1); }

parser::Ast_Node_If* parser::Ast_Node_If::generate(Ast* __ast) {

    __ast->print("--> Ast Node If <--");

    __ast->tokens_position++;

    Name_Space* _body_name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _body_code_block = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_body_code_block) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _body_name_space
    );

    Ast_Node_If* _if_node = (Ast_Node_If*) malloc(sizeof(Ast_Node_If));

    new (_if_node) Ast_Node_If(
        _body_code_block
    );

    __ast->open_nodes->add(
        _body_code_block, 0
    );

    __ast->add_to_chain(
        _body_name_space
    );

    _if_node->set_condition(__ast);

    _if_node->set_body(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node If End <--");

    return _if_node;

}



parser::Ast_Node_Else_If::~Ast_Node_Else_If() { body->~Ast_Node_Code_Block(); free(body); condition->~Ast_Node_Variable_Declaration(); free(condition); }

parser::Ast_Node_Else_If::Ast_Node_Else_If(Ast_Node_Code_Block* __body) : Ast_Node(AST_NODE_ELSE_IF, 0), body(__body), next_if_statement(0) {}

void parser::Ast_Node_Else_If::set_condition(Ast* __ast) {

    if (__ast->get_token(0)->id != OPEN_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), OPEN_PARENTHESIS);

    __ast->tokens_position++;

    condition = parser::get_condition(__ast);

    if (__ast->get_token(0)->id != CLOSE_PARENTHESIS) throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), CLOSE_PARENTHESIS);

    __ast->tokens_position++;

}

void parser::Ast_Node_Else_If::set_body(Ast* __ast) { body->set_code(__ast, 1); }

parser::Ast_Node_Else_If* parser::Ast_Node_Else_If::generate(Ast* __ast) {

    __ast->print("--> Ast Node Else If <--");

    if (
        !__ast->current_nodes->count ||
        __ast->current_nodes->last->object->node_type != AST_NODE_IF && 
        __ast->current_nodes->last->object->node_type != AST_NODE_ELSE_IF
    ) throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0));

    else {

        switch (__ast->current_nodes->last->object->node_type)
        {
        case AST_NODE_IF:
            
            (
                (Ast_Node_If*) __ast->current_nodes->last->object
            )->next_if_statement = 1;

            break;

        case AST_NODE_ELSE_IF:

            (
                (Ast_Node_Else_If*) __ast->current_nodes->last->object
            )->next_if_statement = 1;

            break;
        
        default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

    __ast->tokens_position++;

    Name_Space* _body_name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _body_code_block = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_body_code_block) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _body_name_space
    );

    Ast_Node_Else_If* _else_if_node = (Ast_Node_Else_If*) malloc(sizeof(Ast_Node_Else_If));

    new (_else_if_node) Ast_Node_Else_If(
        _body_code_block
    );

    __ast->open_nodes->add(
        _body_code_block, 0
    );

    __ast->add_to_chain(
        _body_name_space
    );

    _else_if_node->set_condition(__ast);

    _else_if_node->set_body(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node Else If End <--");

    return _else_if_node;

}



parser::Ast_Node_Else::~Ast_Node_Else() { body->~Ast_Node_Code_Block(); free(body); }

parser::Ast_Node_Else::Ast_Node_Else(Ast_Node_Code_Block* __body) : Ast_Node(AST_NODE_ELSE, 0), body(__body) {}

void parser::Ast_Node_Else::set_body(Ast* __ast) { body->set_code(__ast, 1); }

parser::Ast_Node_Else* parser::Ast_Node_Else::generate(Ast* __ast) {

    __ast->print("--> Ast Node Else <--");

    if (
        !__ast->current_nodes->count ||
        __ast->current_nodes->last->object->node_type != AST_NODE_IF && 
        __ast->current_nodes->last->object->node_type != AST_NODE_ELSE_IF
    ) throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0));

    else {

        switch (__ast->current_nodes->last->object->node_type)
        {
        case AST_NODE_IF:
            
            (
                (Ast_Node_If*) __ast->current_nodes->last->object
            )->next_if_statement = 1;

            break;

        case AST_NODE_ELSE_IF:

            (
                (Ast_Node_Else_If*) __ast->current_nodes->last->object
            )->next_if_statement = 1;

            break;
        
        default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

    __ast->tokens_position++;

    Name_Space* _body_name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _body_code_block = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_body_code_block) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _body_name_space
    );

    Ast_Node_Else* _else_node = (Ast_Node_Else*) malloc(sizeof(Ast_Node_Else));

    new (_else_node) Ast_Node_Else(
        _body_code_block
    );

    __ast->open_nodes->add(
        _body_code_block, 0
    );

    __ast->add_to_chain(
        _body_name_space
    );

    _else_node->set_body(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node Else End <--");

    return _else_node;

}



parser::Ast_Node_For::~Ast_Node_For() { delete variable_declarations; delete execution; condition->~Ast_Node_Variable_Declaration(); free(condition);  body->~Ast_Node_Code_Block(); free(body); }

parser::Ast_Node_For::Ast_Node_For(Ast_Node_Code_Block* __body) : Ast_Node(AST_NODE_FOR, 0), body(__body), variables_declarations_size(0) {}

void parser::Ast_Node_For::set_variable_declarations(Ast* __ast) {

    __ast->print("--> Ast Node For -- Variable Declarations <--");

    if (__ast->get_token(0)->id == END_INSTRUCTION) { 
        
        variable_declarations = 
            new utils::Linked_List <Ast_Node*>(0);
        
        __ast->tokens_position++; return;}

    __ast->open_nodes->add(
        body, 0
    );

    variable_declarations = 
        Ast_Node_Variable_Declaration::generate(__ast);

    for (int _ = 0; _ < variable_declarations->count; _++)

        variables_declarations_size += 
            variable_declarations->operator[](_)->representive_type->get_size();

    variable_declarations->destroy_content = 1;

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->print("--> Ast Node For -- Variable Declarations End <--");

}

void parser::Ast_Node_For::set_condition(Ast* __ast) {

    __ast->print("--> Ast Node For -- Condition <--");

    __ast->open_nodes->add(
        body, 0
    );

    condition = 
        parser::get_condition(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    if (__ast->get_token(0)->id != END_INSTRUCTION) 
    
        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), END_INSTRUCTION);

    __ast->tokens_position++;

    __ast->print("--> Ast Node For -- Condition End <--");

}

void parser::Ast_Node_For::set_execution(Ast* __ast) {

    __ast->print("--> Ast Node For -- Execution <--");

    execution = new utils::Linked_List <Ast_Node_Expression*>();

    __ast->open_nodes->add(
        body, 0
    );

    while(__ast->get_token(0)->id != CLOSE_PARENTHESIS) {

        execution->add(
            Ast_Node_Expression::generate(
                __ast
            )
        );

        switch (__ast->get_token(0)->id)
        {
        case CLOSE_PARENTHESIS: break;
        case COMMA: __ast->tokens_position++; break;
        default: throw Unexpected_Token_Ast(__ast->code_information, __ast->get_token(0)); break;
        }

    }

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->tokens_position++;

    __ast->print("--> Ast Node For -- Execution End <--");

}

void parser::Ast_Node_For::set_body(Ast* __ast) { 
    
    __ast->print("--> Ast Node For -- Body <--");

    __ast->open_nodes->add(
        body, 0
    );
    
    body->set_code(__ast, 1); 

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );
    
    __ast->print("--> Ast Node For -- Body End <--");

}

parser::Ast_Node_For* parser::Ast_Node_For::generate(Ast* __ast) {

    __ast->print("--> Ast Node For <--");

    __ast->tokens_position++;

    if (__ast->get_token(0)->id != OPEN_PARENTHESIS)

        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), OPEN_PARENTHESIS);

    __ast->tokens_position++;

    Name_Space* _body_name_space = new Name_Space(
        new utils::Linked_List <char*>(),
        NAME_SPACE_TYPE_CODE_BLOCK
    );

    Ast_Node_Code_Block* _body_code_block = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    new (_body_code_block) Ast_Node_Code_Block(
        (Ast_Node_Code_Block*) __ast->open_nodes->last->object,
        _body_name_space
    );

    Ast_Node_For* _for_node = (Ast_Node_For*) malloc(sizeof(Ast_Node_For));

    new (_for_node) Ast_Node_For(
        _body_code_block
    );

    __ast->open_nodes->add(
        _for_node, 0
    );

    __ast->add_to_chain(
        _body_name_space
    );

    _for_node->set_variable_declarations(__ast);

    _for_node->set_condition(__ast);

    _for_node->set_execution(__ast);

    _for_node->set_body(__ast);

    delete __ast->open_nodes->remove(
        __ast->open_nodes->count
    );

    __ast->pop_from_chain();

    __ast->print("--> Ast Node For End <--");

    return _for_node;

}



parser::Ast_Node_Control_Structs_Key_Words::~Ast_Node_Control_Structs_Key_Words() {}

parser::Ast_Node_Control_Structs_Key_Words::Ast_Node_Control_Structs_Key_Words(Ast_Node* __control_struct, char __key_word_id) 
    : Ast_Node(AST_NODE_CONTROL_STRUCTS_KEY_WORD, 0), control_struct(__control_struct), key_word_id(__key_word_id) {}

parser::Ast_Node_Control_Structs_Key_Words* parser::Ast_Node_Control_Structs_Key_Words::generate(Ast* __ast) {

    __ast->print("--> Ast Node Control Structs Key Words <--");

    char _key_word_id = __ast->get_token(0)->id;

    utils::Data_Linked_List <Ast_Node*>* _current_open_node = __ast->open_nodes->last;
    Ast_Node* _control_struct = 0;

    while(_current_open_node) {

        if (
            parser::is_control_struct(_current_open_node->object->node_type)
        ) { _control_struct = _current_open_node->object; break; }

        _current_open_node = _current_open_node->previous;

    }

    if (!_control_struct) 

        throw Control_Struct_Key_Word_Out_Of_Control_Struct(__ast->code_information, __ast->get_token(0));

    __ast->tokens_position++;

    Ast_Node_Control_Structs_Key_Words* _control_structs_key_words = 
        (Ast_Node_Control_Structs_Key_Words*) malloc(sizeof(Ast_Node_Control_Structs_Key_Words));

    new (_control_structs_key_words) Ast_Node_Control_Structs_Key_Words(
        _control_struct, _key_word_id
    );

    if (__ast->get_token(0)->id != END_INSTRUCTION)

        throw Expected_Token_Ast(__ast->code_information, __ast->get_token(0), END_INSTRUCTION);

    __ast->tokens_position++;

    __ast->print("--> Ast Node Control Structs Key Words End <--");

    return _control_structs_key_words;

}



parser::Ast_Node_Type_Conversion::~Ast_Node_Type_Conversion() { delete representive_type; value->~Ast_Node(); free(value); }

parser::Ast_Node_Type_Conversion::Ast_Node_Type_Conversion(Type_Information* __representive_value, Ast_Node* __value) 
    : Ast_Node(AST_NODE_TYPE_CONVERSION, __representive_value), value(__value) {}

parser::Ast_Node_Type_Conversion* parser::Ast_Node_Type_Conversion::generate(Ast* __ast) {

    __ast->print(" --> Ast Node Type Conversion <--");

    __ast->tokens_position++;

    Type_Information* _type_conversion = 
        Type_Information::generate(__ast, 1);

    if (
        __ast->get_token(0)->id != CLOSE_BRACES
    )
        throw Expected_Token_Ast(
            __ast->code_information,
            __ast->get_token(0),
            CLOSE_BRACES
        );

    __ast->tokens_position++;

    if (
        __ast->get_token(0)->id != CLOSE_BRACES
    )
        throw Expected_Token_Ast(
            __ast->code_information,
            __ast->get_token(0),
            CLOSE_BRACES
        );

    __ast->tokens_position++;

    Ast_Node* _value = 
        get_value(__ast);

    Ast_Node_Type_Conversion* _type_conversion_node = 
        (Ast_Node_Type_Conversion*) malloc(sizeof(Ast_Node_Type_Conversion));

    new (_type_conversion_node) Ast_Node_Type_Conversion(
        _type_conversion, _value
    );

    __ast->print(" --> Ast Node Type Conversion End <--");

    return _type_conversion_node;

}
