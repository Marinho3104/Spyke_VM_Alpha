#include "convertor_ast.h"

#include "convertor_exceptions.h"
#include "program_definitions.h"
#include "token_definitions.h"
#include "convertor_helper.h"
#include "ast_definitions.h"
#include "linked_list.h"
#include "ast_helper.h"
#include "byte_code.h"
#include "convertor.h"
#include "ast_nodes.h"
#include "token.h"
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

        case AST_NODE_PARENTHESIS:

            set_byte_code_of_ast_node_parenthesis(
                __convertor,
                (Ast_Node_Parenthesis*) __ast_node
            ); break;

        case AST_NODE_BYTE_CODE:

            set_byte_code_of_ast_node_byte_code(
                __convertor,
                (Ast_Node_Byte_Code*) __ast_node
            ); break;
            
        case AST_NODE_RETURN_KEY_WORD:

            set_byte_code_of_ast_node_return_key_word(
                __convertor,
                (Ast_Node_Return_Key_Word*) __ast_node
            ); break;

        case AST_NODE_CONSTRUCTOR_CALL:

            set_byte_code_of_ast_node_constructor_call(
                __convertor,
                (Ast_Node_Constructor_Call*) __ast_node
            ); break;

        case AST_NODE_ACCESSING:

            set_byte_code_of_ast_node_accessing(
                __convertor,
                (Ast_Node_Accessing*) __ast_node,
                1
            ); break;

        case AST_NODE_CAST:

            set_byte_code_of_ast_node_cast(
                __convertor,
                (Ast_Node_Cast*) __ast_node
            ); break;

        case AST_NODE_IF:

            set_byte_code_of_ast_node_if(
                __convertor,
                (Ast_Node_If*) __ast_node
            ); break;  

        case AST_NODE_ELSE_IF:

            set_byte_code_of_ast_node_else_if(
                __convertor,
                (Ast_Node_Else_If*) __ast_node
            ); break;  

        case AST_NODE_ELSE:

            set_byte_code_of_ast_node_else(
                __convertor,
                (Ast_Node_Else*) __ast_node
            ); break;  
        case AST_NODE_WHILE:

            set_byte_code_of_ast_node_while(
                __convertor,
                (Ast_Node_While*) __ast_node
            ); break;  

        case AST_NODE_DO_WHILE:

            set_byte_code_of_ast_node_do_while(
                __convertor,
                (Ast_Node_Do_While*) __ast_node
            ); break; 

        case AST_NODE_CONTROL_STRUCTS_KEY_WORD:

            set_byte_code_of_ast_node_control_structs_key_words(
                __convertor,
                (Ast_Node_Control_Structs_Key_Words*) __ast_node
            ); break;

        case AST_NODE_FOR:

            set_byte_code_of_ast_node_for(
                __convertor,
                (Ast_Node_For*) __ast_node
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

 /// ///

    // Allocate memory for return value and return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            __ast_node_function_declaration->representive_type->get_size() + 2
        )
    );

    __convertor->current_block->current_stack_size = 
        __ast_node_function_declaration->representive_type->get_size() + 2;

    // Make copies for stack parameters variables 
    for (int _ = 0; _ < __ast_node_function_declaration->parameters->count; _++) {

        // Set stack size
        ((Ast_Node_Variable_Declaration*) __ast_node_function_declaration->parameters->operator[](_))->stack_position = 
            __convertor->current_block->current_stack_size;

        __convertor->current_block->current_stack_size += 
            __ast_node_function_declaration->parameters->operator[](_)->representive_type->get_size();

        // Load for parameters copy
        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                0
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                MEMORY_COPY,
                __ast_node_function_declaration->parameters->operator[](_)->representive_type->get_size()
            )
        );

        // Allocate memory of parameters size
        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_ALLOCATION,
                __ast_node_function_declaration->parameters->operator[](_)->representive_type->get_size()
            )
        );

    }

    // Function Body
    __convertor->set_block(
        __ast_node_function_declaration->body->code
    );


    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size
        )
    );


    // Load address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD_INVERTED,
            __ast_node_function_declaration->representive_type->get_size()
        )
    );


    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
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
        __ast_node_variable_declaration->representive_type->get_size();

    if (__ast_node_variable_declaration->constructor_call)

        set_byte_code_of_ast(
            __convertor,
            __ast_node_variable_declaration->constructor_call->constructor
        );

}

void parser::set_byte_code_of_ast_node_expression(Convertor* __convertor, Ast_Node_Expression* __ast_node_expression) {

    __convertor->print("--> Get Byte Code Of Ast Node Expression <--");

    int _backup_stack_size = 
        __convertor->current_block->current_stack_size;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_expression->expression_instructions
    );

    if (
        __convertor->current_block->current_stack_size != _backup_stack_size && __ast_node_expression->expression_instructions->node_type != AST_NODE_FUNCTION_CALL
    ) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                __convertor->force_memory_allocation ? __ast_node_expression->expression_instructions->representive_type->get_size() : 0
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                __convertor->current_block->current_stack_size - _backup_stack_size
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                MEMORY_COPY,
                __ast_node_expression->expression_instructions->representive_type->get_size()
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_DEALLOCATION,
                __convertor->current_block->current_stack_size - _backup_stack_size - 
                    ( __convertor->force_memory_allocation ? __ast_node_expression->expression_instructions->representive_type->get_size() : 0 )
            )
        );

        __convertor->current_block->current_stack_size = 
            _backup_stack_size +
                    ( __convertor->force_memory_allocation ? __ast_node_expression->expression_instructions->representive_type->get_size() : 0 );

    }

}

void parser::set_byte_code_of_ast_node_variable(Convertor* __convertor, Ast_Node_Variable* __ast_node_variable) { // Here  <------

    __convertor->print("--> Get Byte Code Of Ast Node Variable <--");

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            __convertor->current_block->current_stack_size - __ast_node_variable->variable_declaration->stack_position
        )
    );

    if (__convertor->only_call) return;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY,
            __ast_node_variable->representive_type->get_size()
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

    if (__convertor->only_call) return;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY,
            __ast_node_implicit_value->representive_type->get_size()
        )
    );

}

void parser::set_byte_code_of_ast_node_function_call(Convertor* __convertor, Ast_Node_Function_Call* __ast_node_function_call) {

    __convertor->print("--> Get Byte Code Of Ast Node Function Call <--");

    std::cout << "Function name -> " << __ast_node_function_call->function_declaration->body_position << std::endl;


    // Get inicial stack size
    int _backup_stack_size =
        __convertor->current_block->current_stack_size;


    // Previous value of call var
    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;


    // Load all parameters given
    for (int _ = __ast_node_function_call->parameters->count - 1; _ >= 0; _--)

        set_byte_code_of_ast(
            __convertor,
            __ast_node_function_call->parameters->operator[](_)->expression_instructions
        );


    // Set previous values
    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;


    // Load address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD_INVERTED,
            __ast_node_function_call->function_declaration->representive_type->get_size()
        )
    );


    // Memory copy of address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );


    // Call function
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            __ast_node_function_call->function_declaration->body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );


    // If there was some "extra" stack allocation (eg. for functions returns), copies function return value into "first slot"
    if (
        __convertor->current_block->current_stack_size != _backup_stack_size
    ) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                0
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                __convertor->current_block->current_stack_size - _backup_stack_size
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                MEMORY_COPY,
                __ast_node_function_call->function_declaration->representive_type->get_size()
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_DEALLOCATION,
                __convertor->current_block->current_stack_size - _backup_stack_size
            )
        );

    }


    // Forced memory allocation
    if (__convertor->force_memory_allocation) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_ALLOCATION,
                __ast_node_function_call->function_declaration->representive_type->get_size()
            )
        );

        _backup_stack_size += 
            __ast_node_function_call->function_declaration->representive_type->get_size();

    }


    // Only Call
    if (__convertor->only_call) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                __convertor->force_memory_allocation ? __ast_node_function_call->function_declaration->representive_type->get_size()
                    : 0
            )
        );

    }

    __convertor->current_block->current_stack_size = 
        _backup_stack_size;

}

void parser::set_byte_code_of_ast_node_pointer_operations(Convertor* __convertor, Ast_Node_Pointer_Operation* __ast_node_pointer_operation) {

    __convertor->print("--> Get Byte Code Of Ast Node Pointer Operations <--");

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1; // <-------- Here
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_pointer_operation->value
    );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    if (__ast_node_pointer_operation->pointer_level > 0) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                0
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                MEMORY_COPY_STACK_FRAME,
                0
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                0
            )
        );

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_ALLOCATION,
                2
            )
        );

        __convertor->current_block->current_stack_size += 2;

    }

    else {

        for (int _ = 0; _ < __ast_node_pointer_operation->pointer_level * -1; _++)

            __convertor->current_block->block->add(
                byte_code::Byte_Code::generate(
                    GET_ADDRESS_FROM_STACK,
                    0
                )
            );

    }


}

void parser::set_byte_code_of_ast_node_parenthesis(Convertor* __convertor, Ast_Node_Parenthesis* __ast_node_parenthesis) {

    __convertor->print("--> Get Byte Code Of Ast Node Pointer Operations <--");

    set_byte_code_of_ast(
        __convertor,
        __ast_node_parenthesis->expression
    );

}

void parser::set_byte_code_of_ast_node_byte_code(Convertor* __convertor, Ast_Node_Byte_Code* __ast_node_byte_code) {

    __convertor->print("--> Get Byte Code Of Ast Node Byte Code <--");

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 0;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 0;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_byte_code->code
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            1
        )
    );

    __convertor->current_block->current_stack_size ++;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_byte_code->argument
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            3
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            2
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            3
        )
    ); 

    __convertor->current_block->current_stack_size -= 3;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CUSTOM_BYTE_CODE,
            0
        )
    );

}

void parser::set_byte_code_of_ast_node_return_key_word(Convertor* __convertor, Ast_Node_Return_Key_Word* __ast_node_return_key_word) {

    __convertor->print("--> Get Byte Code Of Ast Node Return Key Word <--");

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_return_key_word->expression
    );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            __convertor->current_block->current_stack_size
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY,
            __ast_node_return_key_word->expression->representive_type->get_size()
        )
    ); 

}

void parser::set_byte_code_of_ast_node_constructor_call(Convertor* __convertor, Ast_Node_Constructor_Call* __ast_node_constructor_call) {

    __convertor->print("--> Get Byte Code Of Ast Node Constructor Call <--");

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            __ast_node_constructor_call->representive_type->get_size()
        )
    );

    __ast_node_constructor_call->this_variable->stack_position = 
        __convertor->current_block->current_stack_size;

    __convertor->current_block->current_stack_size +=
        __ast_node_constructor_call->representive_type->get_size();

    // Previous value of call var
    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 0;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 0;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_constructor_call->constructor
    );

    // Set previous values
    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    if (__convertor->only_call) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                LOAD,
                __ast_node_constructor_call->representive_type->get_size()
            )
        );

    }

    if (!__convertor->force_memory_allocation) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_DEALLOCATION,
                __ast_node_constructor_call->representive_type->get_size()
            )
        );  

        __convertor->current_block->current_stack_size -=
            __ast_node_constructor_call->representive_type->get_size();

    }

}

void parser::set_byte_code_of_ast_node_accessing(Convertor* __convertor, Ast_Node_Accessing* __ast_node_accessing, bool __first) {

    __convertor->print("--> Get Byte Code Of Ast Node Accessing <--");

    int _backup_stack_size_with_first = 
        __convertor->current_block->current_stack_size;

    if (__first) {

        bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
        bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

        set_byte_code_of_ast(
            __convertor,
            __ast_node_accessing->value
        );

        __convertor->force_memory_allocation = _prev_force_allocation_value;
        __convertor->only_call = _prev_call_value;

    }

    int _backup_stack_size = 
        __convertor->current_block->current_stack_size;

    switch (__ast_node_accessing->accessing->node_type)
    {
    
    case AST_NODE_VARIABLE: 

        __convertor->current_block->block->last->object->argument -= 
            ((Ast_Node_Variable*)__ast_node_accessing->value)->variable_declaration->representive_type->declaration->get_variable_off(
                ((Ast_Node_Variable*)__ast_node_accessing->accessing)->variable_declaration
            ); break;

    case AST_NODE_FUNCTION_CALL:

        delete __convertor->current_block->block->remove(
            __convertor->current_block->block->count
        );

        set_byte_code_of_ast(
            __convertor,
            __ast_node_accessing->accessing
        ); 
            
        break;

    case AST_NODE_ACCESSING:

        switch ( ( (Ast_Node_Accessing*)__ast_node_accessing->accessing )->value->node_type )
        {

        case AST_NODE_VARIABLE:

            __convertor->current_block->block->last->object->argument -= 
                ((Ast_Node_Variable*)__ast_node_accessing->value)->variable_declaration->representive_type->declaration->get_variable_off(
                    ((Ast_Node_Variable*) ( (Ast_Node_Accessing*) __ast_node_accessing->accessing)->value )->variable_declaration
                );

            ((Ast_Node_Variable*)__ast_node_accessing->value)->variable_declaration->stack_position = 
                __convertor->current_block->current_stack_size - __convertor->current_block->block->last->object->argument;
        
            break;
        
        case AST_NODE_FUNCTION_CALL:

            {

                delete __convertor->current_block->block->remove(
                    __convertor->current_block->block->count
                );

                bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
                bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

                set_byte_code_of_ast(
                    __convertor,
                    __ast_node_accessing->accessing
                ); 

                __convertor->force_memory_allocation = _prev_force_allocation_value;
                __convertor->only_call = _prev_call_value;
                
                break;

            }

        default: exit(1); break;

        }

        set_byte_code_of_ast_node_accessing(
            __convertor,
            (Ast_Node_Accessing*) __ast_node_accessing->accessing,
            0
        );

        break;

    default: exit(1); break;

    }

}

void parser::set_byte_code_of_ast_node_cast(Convertor* __convertor, Ast_Node_Cast* __ast_node_cast) {

    set_byte_code_of_ast(
        __convertor,
        __ast_node_cast->constructor_call
    );

}

void parser::set_byte_code_of_ast_node_while(Convertor* __convertor, Ast_Node_While* __ast_node_while) {

    int _body_position = 
        __convertor->add_block();

    __ast_node_while->body_position = _body_position;

    int _prev_stack_position = 
        __convertor->current_block->current_stack_size;

    __ast_node_while->previous_stack_position = _prev_stack_position;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](_body_position);

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    __convertor->current_block->current_stack_size = 
        _prev_stack_position;

    
    // Return address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;


    // Condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_while->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CHECK_NOT,
            3 // Instructions count to jumps
        )
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
    );

    
    __convertor->set_block(
        __ast_node_while->body->code
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );
 
    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );


    __convertor->current_block = 
        _previous_code_block;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );

}

void parser::set_byte_code_of_ast_node_do_while(Convertor* __convertor, Ast_Node_Do_While* __ast_node_do_while) {

    int _body_position = 
        __convertor->add_block();

    __ast_node_do_while->body_position = _body_position;

    int _prev_stack_position = 
        __convertor->current_block->current_stack_size;

    __ast_node_do_while->previous_stack_position = _prev_stack_position;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](_body_position);

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    __convertor->current_block->current_stack_size = 
        _prev_stack_position;

    
    // Return address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;

    __convertor->set_block(
        __ast_node_do_while->body->code
    );

    // Condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_do_while->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CHECK_NOT,
            3 // Instructions count to jumps
        )
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );
 
    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );


    __convertor->current_block = 
        _previous_code_block;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );

}

void parser::set_byte_code_of_ast_node_if(Convertor* __convertor, Ast_Node_If* __ast_node_if) {

    int _body_position = 
        __convertor->add_block();

    int _prev_stack_position = 
        __convertor->current_block->current_stack_size;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](_body_position);

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    __convertor->current_block->current_stack_size = 
        _prev_stack_position;

    
    // Return address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;


    // Condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_if->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CHECK_NOT,
            3 // Instructions count to jumps
        )
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            __ast_node_if->next_if_statement ? 1 : 0
        )
    );

    
    __convertor->set_block(
        __ast_node_if->body->code
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
    );
 


    __convertor->current_block = 
        _previous_code_block;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );

}

void parser::set_byte_code_of_ast_node_else_if(Convertor* __convertor, Ast_Node_Else_If* __ast_node_else_if) {

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            JUMP,
            3
        )
    );

    int _body_position = 
        __convertor->add_block();

    int _prev_stack_position = 
        __convertor->current_block->current_stack_size;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](_body_position);

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    __convertor->current_block->current_stack_size = 
        _prev_stack_position;

    
    // Return address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;


    // Condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_else_if->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CHECK_NOT,
            3 // Instructions count to jumps
        )
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            __ast_node_else_if->next_if_statement ? 1 : 0
        )
    );

    
    __convertor->set_block(
        __ast_node_else_if->body->code
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
    );
 


    __convertor->current_block = 
        _previous_code_block;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );

}

void parser::set_byte_code_of_ast_node_else(Convertor* __convertor, Ast_Node_Else* __ast_node_else) {

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            JUMP,
            3
        )
    );

    int _body_position = 
        __convertor->add_block();

    int _prev_stack_position = 
        __convertor->current_block->current_stack_size;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](_body_position);

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    __convertor->current_block->current_stack_size = 
        _prev_stack_position;

    
    // Return address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;

    __convertor->set_block(
        __ast_node_else->body->code
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
    );
 


    __convertor->current_block = 
        _previous_code_block;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );

}

void parser::set_byte_code_of_ast_node_control_structs_key_words(
    Convertor* __convertor, Ast_Node_Control_Structs_Key_Words* __ast_node_control_structs_key_words) {

        int _previous_stack_position = 0, _body_position = 0;

        switch (__ast_node_control_structs_key_words->control_struct->node_type)
        {
        case AST_NODE_WHILE: 

            _previous_stack_position = ((Ast_Node_While*) __ast_node_control_structs_key_words->control_struct)->previous_stack_position; 
            _body_position = ((Ast_Node_While*) __ast_node_control_structs_key_words->control_struct)->body_position; 
            break;

        case AST_NODE_DO_WHILE: 
            
            _previous_stack_position = ((Ast_Node_Do_While*) __ast_node_control_structs_key_words->control_struct)->previous_stack_position; break;
            _body_position = ((Ast_Node_Do_While*) __ast_node_control_structs_key_words->control_struct)->body_position; 
            break;

        case AST_NODE_FOR: 
        
            _previous_stack_position = ((Ast_Node_For*) __ast_node_control_structs_key_words->control_struct)->previous_stack_position; break;
            _body_position = ((Ast_Node_For*) __ast_node_control_structs_key_words->control_struct)->body_position; 
            break;

        default: break;
        }

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_DEALLOCATION,
                __convertor->current_block->current_stack_size - _previous_stack_position
            )
        );

        switch (__ast_node_control_structs_key_words->key_word_id)
        {

            case BREAK: 
            
                __convertor->current_block->block->add(
                    byte_code::Byte_Code::generate(
                        LOAD,
                        0
                    )
                );

                __convertor->current_block->block->add(
                    byte_code::Byte_Code::generate(
                        GO_STACK_FRAME,
                        0
                    )
                );

                break;

            case CONTINUE: 
            
                __convertor->current_block->block->add(
                    byte_code::Byte_Code::generate(
                        GO,
                        _body_position
                    )
                );
            
                __convertor->call_byte_codes->add(
                    __convertor->current_block->block->last->object
                );
            
                break;
        
            default: break;
        }

}

void parser::set_byte_code_of_ast_node_for(Convertor* __convertor, Ast_Node_For* __ast_node_for) {

    int _body_position = 
        __convertor->add_block();

    __ast_node_for->body_position = _body_position;

    int _prev_stack_position = 
        __convertor->current_block->current_stack_size;

    __ast_node_for->previous_stack_position = _prev_stack_position;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](_body_position);

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    __convertor->current_block->current_stack_size = 
        _prev_stack_position;

    // //

    // Return address 
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_ALLOCATION,
            2
        )
    );

    __convertor->current_block->current_stack_size += 2;

    for (int _ = 0; _ < __ast_node_for->variable_declarations->count; _++)

        set_byte_code_of_ast(
            __convertor,
            __ast_node_for->variable_declarations->operator[](_)
        );

    __ast_node_for->variable_declarations_instructions_count = 
        __convertor->current_block->block->count;


    // Condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_for->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            CHECK_NOT,
            3 // Instructions count to jumps
        )
    );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position
        )
    );

    // Load return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    // Goto to return address
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO_STACK_FRAME,
            0
        )
    );


    __convertor->set_block(
        __ast_node_for->body->code
    );

    // Set execution
    for (int _ = 0; _ < __ast_node_for->execution->count; _++)

        set_byte_code_of_ast(
            __convertor,
            __ast_node_for->execution->operator[](_)
        );

    // Deallocation
    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            STACK_MEMORY_DEALLOCATION,
            __convertor->current_block->current_stack_size - _prev_stack_position - __ast_node_for->variables_declarations_size - 2
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );
 
    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );

    __ast_node_for->go_back = 
        __convertor->current_block->block->last->object;

    __convertor->for_control_struct_go_back->add(
        __ast_node_for
    );



    __convertor->current_block = 
        _previous_code_block;

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            LOAD,
            0
        )
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            MEMORY_COPY_ADDRESS,
            0
        )
    );

    __convertor->memory_copy_stack_frame_before_go->add(
        __convertor->current_block->block->last->object
    );

    __convertor->current_block->block->add(
        byte_code::Byte_Code::generate(
            GO,
            _body_position
        )
    );

    __convertor->call_byte_codes->add(
        __convertor->current_block->block->last->object
    );
    

}






