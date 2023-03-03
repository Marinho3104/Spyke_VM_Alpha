#include "convertor_ast.h"

#include "convertor_exceptions.h"
#include "program_definitions.h"
#include "convertor_helper.h"
#include "ast_definitions.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "byte_code.h"
#include "convertor.h"
#include "ast_nodes.h"
#include "opcodes.h"
#include <iostream>


void parser::set_byte_code_of_ast(Convertor* __convertor, Ast_Node* __ast_node) {

    switch (__ast_node->node_type)
    {
        case AST_NODE_NAME_SPACE: 

            set_byte_code_of_ast_node_name_space(
                __convertor, 
                (Ast_Node_Name_Space*) __ast_node
            ); break;

        case AST_NODE_CODE_BLOCK: 

            set_byte_code_of_ast_node_code_block(
                __convertor, 
                (Ast_Node_Code_Block*) __ast_node
            ); break;

        case AST_NODE_STRUCT_DECLARATION:

            set_byte_code_of_ast_node_struct_declaration(
                __convertor,
                (Ast_Node_Struct_Declaration*) __ast_node
            ); break;

        case AST_NODE_FUNCTION_DECLARATION:

            set_byte_code_of_ast_node_function_declaration(
                __convertor,
                (Ast_Node_Function_Declaration*) __ast_node
            ); break;

        case AST_NODE_VARIABLE_DECLARATION:

            set_byte_code_of_ast_node_variable_declaration(
                __convertor,
                (Ast_Node_Variable_Declaration*) __ast_node
            ); break;

        case AST_NODE_EXPRESSION:

            set_byte_code_of_ast_node_expression(
                __convertor,
                (Ast_Node_Expression*) __ast_node
            ); break;

        case AST_NODE_VARIABLE:

            set_byte_code_of_ast_node_variable(
                __convertor,
                (Ast_Node_Variable*) __ast_node
            ); break;

        case AST_NODE_IMPLICIT_VALUE:

            set_byte_code_of_ast_node_implicit_value(
                __convertor,
                (Ast_Node_Implicit_Value*) __ast_node
            ); break;

        case AST_NODE_FUNCTION_CALL:

            set_byte_code_of_ast_node_function_call(
                __convertor,
                (Ast_Node_Function_Call*) __ast_node
            ); break;

        case AST_NODE_POINTER_OPERATION:

            set_byte_code_of_ast_node_pointer_operations(
                __convertor,
                (Ast_Node_Pointer_Operation*) __ast_node
            ); break;

        default: throw Ordinary_Exception_Convertor("Unexpected Ast Node Type"); break;
    }

}

void parser::set_byte_code_of_ast_node_name_space(Convertor* __convertor, Ast_Node_Name_Space* __ast_node_name_space) {

    byte_code::Byte_Code_Block* _previous_byte_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        __convertor->blocks->operator[](0);

    __convertor->print("--> Get Byte Code Of Ast Node Name Space <--");

    for (int _ = 0; _ < __ast_node_name_space->declarations->count; _++)

        set_byte_code_of_ast(
            __convertor,
            __ast_node_name_space->declarations->operator[](_)
        );

    __convertor->current_block =
        _previous_byte_code_block;

}

void parser::set_byte_code_of_ast_node_code_block(Convertor* __convertor, Ast_Node_Code_Block* __ast_node_code_block) {

    __convertor->print("--> Get Byte Code Of Ast Node Code Block <--");

    __convertor->set_block(
        __ast_node_code_block->code
    );

}

void parser::set_byte_code_of_ast_node_struct_declaration(Convertor* __convertor, Ast_Node_Struct_Declaration* __ast_node_struct_declaration) {

    __convertor->print("--> Get Byte Code Of Ast Node Struct Declaration <--");

    if (!__ast_node_struct_declaration->body_defined) return;

    for (int _ =  0; _ < __ast_node_struct_declaration->body->declarations->count; _++)

        if (
            __ast_node_struct_declaration->body->declarations->operator[](_)->node_type == AST_NODE_FUNCTION_DECLARATION
        ) 
            set_byte_code_of_ast_node_function_declaration(
                __convertor,
                (Ast_Node_Function_Declaration*) __ast_node_struct_declaration->body->declarations->operator[](_)
            );

}

void parser::set_byte_code_of_ast_node_function_declaration(Convertor* __convertor, Ast_Node_Function_Declaration* __ast_node_function_declaration) {

    __convertor->print("--> Get Byte Code Of Ast Node Function Declaration <--");

    if (__ast_node_function_declaration->body_position == -1)

        __ast_node_function_declaration->body_position =
            __convertor->add_block();

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](__ast_node_function_declaration->body_position);

    _byte_code_block->entry_point = 1;

    if (!__ast_node_function_declaration->body_defined) return;

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    _byte_code_block->current_stack_size += __ast_node_function_declaration->representive_type->get_size();

    if (_byte_code_block->current_stack_size)

        _byte_code_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_ALLOCATION,
                _byte_code_block->current_stack_size
            )
        );


    // Return address
    _byte_code_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    _byte_code_block->current_stack_size += 2;

    for (int _ = 0; _ < __ast_node_function_declaration->parameters->count; _++)
    
        set_byte_code_of_ast(
            __convertor,
            __ast_node_function_declaration->parameters->operator[](_)
        );

    __convertor->set_block(
        __ast_node_function_declaration->body->code
    );

    __convertor->current_block = 
        _previous_code_block;

}

void parser::set_byte_code_of_ast_node_variable_declaration(Convertor* __convertor, Ast_Node_Variable_Declaration* __ast_node_variable_declaration) {

    __convertor->print("--> Get Byte Code Of Ast Node Variable Declaration <--");

    __ast_node_variable_declaration->stack_position = 
        __convertor->current_block->current_stack_size;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            __ast_node_variable_declaration->representive_type->get_size()
        )
    );

    __convertor->current_block->current_stack_size += 
        __convertor->current_block->block->last->object->argument;

    // Function call

}

void parser::set_byte_code_of_ast_node_expression(Convertor* __convertor, Ast_Node_Expression* __ast_node_expression) {

    __convertor->print("--> Get Byte Code Of Ast Node Expression <--");

    int _backup_stack_address = 
        __convertor->current_block->current_stack_size;
    
    set_byte_code_of_ast(
        __convertor,
        __ast_node_expression->expression_instructions
    );

    __convertor->current_block->current_stack_size = 
        _backup_stack_address;

}

void parser::set_byte_code_of_ast_node_variable(Convertor* __convertor, Ast_Node_Variable* __ast_node_variable) {

    __convertor->print("--> Get Byte Code Of Ast Node Variable <--");

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            __convertor->current_block->current_stack_size - __ast_node_variable->variable_declaration->stack_position
        )
    );

}

void parser::set_byte_code_of_ast_node_implicit_value(Convertor* __convertor, Ast_Node_Implicit_Value* __ast_node_implicit_value) {

    __convertor->print("--> Get Byte Code Of Ast Node Variable <--");

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD_GLOBAL,
            __convertor->implicit_value->positions->operator[](
                __ast_node_implicit_value->implicit_value_position
            ) + HEAP_MEMORY_SIZE + STACK_MEMORY_SIZE
        )
    );

}

void parser::set_byte_code_of_ast_node_function_call(Convertor* __convertor, Ast_Node_Function_Call* __ast_node_function_call) {

    __convertor->print("--> Get Byte Code Of Ast Node Function Call <--");

    int _off_variable = 
        __ast_node_function_call->function_declaration->representive_type->get_size();

    // Set return address TODO
    _off_variable += 2;

    for (int _ = 0; _ < __ast_node_function_call->parameters->count; _++) {

        set_byte_code_of_ast(
            __convertor,
            __ast_node_function_call->parameters->operator[](_)
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD_INVERTED,
                _off_variable
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                MEMORY_COPY,
                __ast_node_function_call->parameters->operator[](_)->representive_type->get_size()
            )
        );

        _off_variable +=
            __ast_node_function_call->parameters->operator[](_)->representive_type->get_size();

    }

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CALL,
            __ast_node_function_call->function_declaration->body_position
        )
    );

    __convertor->current_block->current_stack_size += 
        __ast_node_function_call->function_declaration->representive_type->get_size();

}

void parser::set_byte_code_of_ast_node_pointer_operations(Convertor* __convertor, Ast_Node_Pointer_Operation* __ast_node_pointer_operation) {

    __convertor->print("--> Get Byte Code Of Ast Node Pointer Operations <--");

    if (!__ast_node_pointer_operation->pointer_level) return;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_pointer_operation->value
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            __ast_node_pointer_operation->pointer_level ? 
                SET_ADDRESS_INTO_STACK :
                    GET_ADDRESS_FROM_STACK,
            0
        )
    );

}



