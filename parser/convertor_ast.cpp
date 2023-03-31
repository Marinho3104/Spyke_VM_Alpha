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

#include <string.h>
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

        // case AST_NODE_CONTROL_STRUCTS_KEY_WORD:

        //     set_byte_code_of_ast_node_control_structs_key_words(
        //         __convertor,
        //         (Ast_Node_Control_Structs_Key_Words*) __ast_node
        //     ); break;

        // case AST_NODE_CONTRACT_DECLARATION: 

        //     break;

        case AST_NODE_FOR:

            set_byte_code_of_ast_node_for(
                __convertor,
                (Ast_Node_For*) __ast_node
            ); break;

        case AST_NODE_TYPE_CONVERSION:

            set_byte_code_of_ast_type_conversion(
                __convertor,
                (Ast_Node_Type_Conversion*) __ast_node
            ); break;
            
        default: std::cout << "Node type -> " << __ast_node->node_type << std::endl; throw Ordinary_Exception_Convertor("Unexpected Ast Node Type"); break;
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
            __ast_node_struct_declaration->body->declarations->operator[](_)->node_type == AST_NODE_FUNCTION_DECLARATION || 
            ( __ast_node_struct_declaration->body->declarations->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION && 
                ( (Ast_Node_Variable_Declaration*) __ast_node_struct_declaration->body->declarations->operator[](_) )->is_static )
        ) 
            set_byte_code_of_ast(
                __convertor,
                __ast_node_struct_declaration->body->declarations->operator[](_)
            );

}

void parser::set_byte_code_of_ast_node_function_declaration(Convertor* __convertor, Ast_Node_Function_Declaration* __ast_node_function_declaration) {

    __convertor->print("--> Get Byte Code Of Ast Node Function Declaration <--");

    if (__ast_node_function_declaration->body_position == -1)

        __ast_node_function_declaration->body_position =
            __convertor->add_block();

    if (
        __ast_node_function_declaration->forward_declaration
    ) 
        __ast_node_function_declaration->forward_declaration->body_position = 
            __ast_node_function_declaration->body_position;

    if (!__ast_node_function_declaration->body_defined) return;

    byte_code::Byte_Code_Block* _byte_code_block =
        __convertor->blocks->operator[](__ast_node_function_declaration->body_position);

    _byte_code_block->entry_point = 1;

    byte_code::Byte_Code_Block* _previous_code_block =
        __convertor->current_block;

    __convertor->current_block = 
        _byte_code_block;

    // // // // //

    // Function return value allocation
    __convertor->set_byte_code_stack_memory_allocation(
        __ast_node_function_declaration->representive_type->get_size()
    );

    // Instruction return address allocation
    __convertor->set_byte_code_stack_memory_allocation(
        2 // Address size
    );

    // Makes copy of given arguments throught stack_frame ( a.k.a LOAD, etc ... )
    for (int _ = 0; _ < __ast_node_function_declaration->parameters->count; _++) {

        // Set parameter stack position
        __convertor->set_variable_declaration_stack_position(
            (Ast_Node_Variable_Declaration*) __ast_node_function_declaration->parameters->operator[](_)
        );

        // Load the position of parameter copy
        __convertor->set_byte_code_load(0);

        // Memory copy of given argument into parameter new position
        __convertor->set_byte_code_memory_copy(
            __ast_node_function_declaration->parameters->operator[](_)->representive_type->get_size()
        );

        // Allocates parameter memory
        __convertor->set_byte_code_stack_memory_allocation(
            __ast_node_function_declaration->parameters->operator[](_)->representive_type->get_size()
        );

    }

    // Set function body byte code
    __convertor->set_block(
        __ast_node_function_declaration->body->code
    );

    // After end of function body it needs to deallocate all stack memory used and go to instruction return address

    // Deallocates all used stack memory
    __convertor->set_byte_code_stack_memory_deallocation(__convertor->current_block->current_stack_size);

    // Loads return instruction address
    __convertor->set_byte_code_load_inverted(
        __ast_node_function_declaration->representive_type->get_size()
    );

    // Go to return address
    __convertor->set_byte_code_go_stack_frame();

    __convertor->current_block = 
        _previous_code_block;

}

void parser::set_byte_code_of_ast_node_variable_declaration(Convertor* __convertor, Ast_Node_Variable_Declaration* __ast_node_variable_declaration) {

    __convertor->print("--> Get Byte Code Of Ast Node Variable Declaration <--");

    // Current block copy
    byte_code::Byte_Code_Block* _previous_code_block = 
        0;

    // If variable is static needs to be "execute" in initial block ( a.k.a block 0 )
    if ( __ast_node_variable_declaration->is_static ) 
        { _previous_code_block = __convertor->current_block; __convertor->current_block = __convertor->blocks->operator[](0); }

    // Set variable stack_position
    __convertor->set_variable_declaration_stack_position(__ast_node_variable_declaration);

    // Allocates the variable size into stack memory
    __convertor->set_byte_code_stack_memory_allocation(
        __ast_node_variable_declaration->representive_type->get_size()
    );

    // Check if it is a "normal" variable or a array
    if (
        __ast_node_variable_declaration->constructor_call && !__ast_node_variable_declaration->array_size
    )

        // Calls variable declaration constructor
        set_byte_code_of_ast(
            __convertor,
            __ast_node_variable_declaration->constructor_call->constructor
        );

    // Execute extra byte code for array size
    else {

        __ast_node_variable_declaration->representive_type->pointer_level--;

        int _variable_size = __ast_node_variable_declaration->representive_type->get_size();

        __ast_node_variable_declaration->representive_type->pointer_level++;

        __convertor->set_variable_declaration_array_size(__ast_node_variable_declaration->array_size, _variable_size);
    
    }

    if (_previous_code_block) __convertor->current_block = _previous_code_block;

}

void parser::set_byte_code_of_ast_node_expression(Convertor* __convertor, Ast_Node_Expression* __ast_node_expression) {

    __convertor->print("--> Get Byte Code Of Ast Node Expression <--");

    // Just executes the set of instructions
    set_byte_code_of_ast(
        __convertor,
        __ast_node_expression->expression_instructions
    );

}

void parser::set_byte_code_of_ast_node_variable(Convertor* __convertor, Ast_Node_Variable* __ast_node_variable) { 

    __convertor->print("--> Get Byte Code Of Ast Node Variable <--");

    // Load the variablde itself 
    // Depending if is global or not ( LOAD, LOAD_GLOBAL )
    __ast_node_variable->variable_declaration->is_static ?
        __convertor->set_byte_code_load_global(
            __ast_node_variable->variable_declaration->stack_position, __convertor->blocks->operator[](0)
        ) :
        __convertor->set_byte_code_load(
            __convertor->current_block->current_stack_size - __ast_node_variable->variable_declaration->stack_position
        );

    // If it is only need to load exit here
    if ( __convertor->only_call ) return;

    // If it reach here it will make a copy of the value into first not allocated stack position

    // Load first not stack allocated  
    __convertor->set_byte_code_load(0); 
    
    // executes the memory copy
    __convertor->set_byte_code_memory_copy( __ast_node_variable->representive_type->get_size() );


}

void parser::set_byte_code_of_ast_node_implicit_value(Convertor* __convertor, Ast_Node_Implicit_Value* __ast_node_implicit_value) {

    __convertor->print("--> Get Byte Code Of Ast Node Variable <--");

    // Load the implicit value itself

    __convertor->set_byte_code_load_global(
        __convertor->implicit_value->positions->operator[](
                __ast_node_implicit_value->implicit_value_position
            ) + HEAP_MEMORY_SIZE + STACK_MEMORY_SIZE,
        __convertor->blocks->operator[](0)
    );

    // If it is only need to load exit here
    if ( __convertor->only_call ) return;

    // If it reach here it will make a copy of the value into first not allocated stack position

    // Load first not stack allocated  
    __convertor->set_byte_code_load(0); 
    
    // executes the memory copy
    __convertor->set_byte_code_memory_copy( __ast_node_implicit_value->representive_type->get_size() );

}

void parser::set_byte_code_of_ast_node_function_call(Convertor* __convertor, Ast_Node_Function_Call* __ast_node_function_call) {

    __convertor->print("--> Get Byte Code Of Ast Node Function Call <--");

    // Save the current stack position ( without function call garbage )
    int _stack_position_backup =    
        __convertor->current_block->current_stack_size;

    // Set the Params in reverse order

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

    for (int _ = __ast_node_function_call->parameters->count - 1; _ >= 0; _--)

        set_byte_code_of_ast(
            __convertor,
            __ast_node_function_call->parameters->operator[](_)
        );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    // Set the return instruction address
    // Needs to load inverted with return size

    // Load inverted the return instruction address
    __convertor->set_byte_code_load_inverted(
        __ast_node_function_call->function_declaration->representive_type->get_size()
    );

    // Memory copy of address
    // Which makes a copy of the given argument in byte code into the loaded value
    __convertor->set_byte_code_memory_copy_address();

    // Go to function declaration address
    __convertor->set_byte_code_go(
        __ast_node_function_call->function_declaration->body_position
    );

    // Check s if there was some extra allocation
    if ( __convertor->current_block->current_stack_size != _stack_position_backup ) {

        // Here need to remove the extra space allocated throught out the parameters
        // and copy the return of the final address into the first one

        // Only if the return value is not spyke type it is worth to memory copy
        if ( ! __ast_node_function_call->representive_type->is_spyke_type() ) {

            // Load the return value of function
            __convertor->set_byte_code_load(0);

            // Load the initial position
            __convertor->set_byte_code_load(
                __convertor->current_block->current_stack_size - _stack_position_backup
            );

            // Memory copy 
            __convertor->set_byte_code_memory_copy(
                __ast_node_function_call->function_declaration->representive_type->get_size()
            );

        }

        // Dealloc all garbage memory
        __convertor->set_byte_code_stack_memory_deallocation(
            __convertor->current_block->current_stack_size - _stack_position_backup
        );

    }

    // Only Call
    if (__convertor->only_call && ! __ast_node_function_call->representive_type->is_spyke_type()) {

        __convertor->set_byte_code_load(
            0
        );

    }

    // Forced memory allocation
    if (__convertor->force_memory_allocation && ! __ast_node_function_call->representive_type->is_spyke_type()) {

        __convertor->current_block->block->add(
            byte_code::Byte_Code::generate(
                STACK_MEMORY_ALLOCATION,
                __ast_node_function_call->function_declaration->representive_type->get_size()
            )
        );

        _stack_position_backup += 
            __ast_node_function_call->function_declaration->representive_type->get_size();

    }

    __convertor->current_block->current_stack_size = 
        _stack_position_backup;

}

void parser::set_byte_code_of_ast_node_pointer_operations(Convertor* __convertor, Ast_Node_Pointer_Operation* __ast_node_pointer_operation) {

    __convertor->print("--> Get Byte Code Of Ast Node Pointer Operations <--");

    // Execute the value

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;
    
    set_byte_code_of_ast(
        __convertor,
        __ast_node_pointer_operation->value
    );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    // Check if is get address from ( & )
    if ( __ast_node_pointer_operation->pointer_level > 0 ) {

        // Load the space of new address
        __convertor->set_byte_code_load(
            0
        );

        // Copies the address of value into new address space
        __convertor->set_byte_code_memory_copy_stack_frame();

        if ( __convertor->only_call )

            __convertor->set_byte_code_load(
                0
            );

        if ( __convertor->force_memory_allocation )

            __convertor->set_byte_code_stack_memory_allocation(
                2 // address size
            );


    }

    else {

        for (int _ = 0; _ < __ast_node_pointer_operation->pointer_level * -1; _++) 
        
            __convertor->set_byte_code_get_address_from_stack();

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

    // Byte code code
    set_byte_code_of_ast(
        __convertor,
        __ast_node_byte_code->code
    );

    // Allocates memory || code ( 1 byte )
    __convertor->set_byte_code_stack_memory_allocation(1);


    // Byte code argument
    set_byte_code_of_ast(
        __convertor,
        __ast_node_byte_code->argument
    );

    // Allocates memory || argument ( 2 byte )
    __convertor->set_byte_code_stack_memory_allocation(2);

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    // Load code
    __convertor->set_byte_code_load(3);

    // Load argument
    __convertor->set_byte_code_load(2);

    // Deallocates the 3 bytes of byte code
    __convertor->set_byte_code_stack_memory_deallocation(3);

    // Set custom byte code
    __convertor->set_byte_code_custom_byte_code(0);

}

void parser::set_byte_code_of_ast_node_return_key_word(Convertor* __convertor, Ast_Node_Return_Key_Word* __ast_node_return_key_word) {

    __convertor->print("--> Get Byte Code Of Ast Node Return Key Word <--");

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 0;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_return_key_word->expression
    );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;

    if ( ! __ast_node_return_key_word->function_return_type->is_spyke_type()) {
    
        // Load the return value of function ( first address of function ) 
        __convertor->set_byte_code_load(
            __convertor->current_block->current_stack_size
        );

        // Copy the loaded value into return value 
        __convertor->set_byte_code_memory_copy(
            __ast_node_return_key_word->expression->representive_type->get_size()
        );
    
    }

}

void parser::set_byte_code_of_ast_node_constructor_call(Convertor* __convertor, Ast_Node_Constructor_Call* __ast_node_constructor_call) {

    __convertor->print("--> Get Byte Code Of Ast Node Constructor Call <--");

    // Here we need to allocate memory for the return value of the contructor

    // Set variable stack position
    __ast_node_constructor_call->this_variable->stack_position = 
        __convertor->current_block->current_stack_size;

    // Allocates variable size
    __convertor->set_byte_code_stack_memory_allocation(
        __ast_node_constructor_call->representive_type->get_size()
    );

    // Execute the constructor itself

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 0;
    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 0;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_constructor_call->constructor
    );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    __convertor->only_call = _prev_call_value;


    if ( __convertor->only_call )

        // Load Variable
        __convertor->set_byte_code_load(
            __ast_node_constructor_call->representive_type->get_size()
        );

    // Cause already allocates the memory of the variable 
    // check if there is no need for the forced allocate and if not deallocate
    if ( ! __convertor->force_memory_allocation )

        __convertor->set_byte_code_stack_memory_deallocation(
            __ast_node_constructor_call->representive_type->get_size()
        );

}

void parser::set_byte_code_of_ast_node_accessing(Convertor* __convertor, Ast_Node_Accessing* __ast_node_accessing, bool __first) {

    __convertor->print("--> Get Byte Code Of Ast Node Accessing <--");

    // Back up stack size
    int _stack_size_back_up = 
        __convertor->current_block->current_stack_size;

    // Executes only first time, for the first value
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

    // Variables for switch code
    Ast_Node_Struct_Declaration* _struct_declaration_node; // For switch -- AST_NODE_VARIABLE
    Ast_Node_Variable* _variable_node; //  For switch -- AST_NODE_VARIABLE
    byte_code::Byte_Code* _load_byte_code = 
        __convertor->current_block->block->last->object; // Byte code of last load
    Ast_Node_Accessing* _accessing_node; // For switch -- AST_NODE_ACCESSING
    short _argument; // For switch

    // Here we have 3 options:
    //
    //      Or variable: meaning is the last accessing and is a variable
    //      Or function call: meaning is the last accessing and is a function call
    //      Or accesssing: meaning there is another accessing -- here we just "prepare" the value for the next accessing value
    //
    switch (__ast_node_accessing->accessing->node_type)
    {

        case AST_NODE_VARIABLE:

            // Save argument
            _argument = 
                 __convertor->current_block->block->last->object->argument;

            // Remove previous load
            delete 
                __convertor->current_block->block->remove(
                    __convertor->current_block->block->count
                );

            // Conversions
            _struct_declaration_node = 
                __ast_node_accessing->value->representive_type->declaration;

            _variable_node = 
                (Ast_Node_Variable*) __ast_node_accessing->accessing;

        ast_node_variable_execution:

            if ( ! _variable_node->variable_declaration->is_static ) {

                // The Idea is, here we have already the variable loaded 
                // so is just to subtract size of the others variables inside the struct until reacth the desired variable

                _variable_node->variable_declaration->stack_position = 
                    _argument - 
                        _struct_declaration_node->get_variable_off(
                            _variable_node->variable_declaration
                        );

            }

            set_byte_code_of_ast(
                __convertor,
                _variable_node
            );

            break;

        case AST_NODE_FUNCTION_CALL: 
        ast_node_function_call_execution:
            // Here just ignore the previous call and just execute the function normally

            // Remove byte code 
            delete 
                __convertor->current_block->block->remove(
                    __convertor->current_block->block->count
                );

            // Function Call
            set_byte_code_of_ast(
                __convertor,
                __ast_node_accessing->accessing
            );

            break;

        case ACCESSING: 
        
            // Conversion
            _accessing_node = 
                (Ast_Node_Accessing*) __ast_node_accessing->accessing;

            switch (_accessing_node->value->node_type)
            {
            case AST_NODE_VARIABLE:

                { 

                    // Conversions
                    _struct_declaration_node = 
                        __ast_node_accessing->value->representive_type->declaration;

                    _variable_node = 
                        (Ast_Node_Variable*) _accessing_node->value;

                    // Cause this information we be necessary for next accessing we specify with force allocation and call
                    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
                    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

                    // Makes normal execution
                    goto 
                        ast_node_variable_execution;

                    __convertor->force_memory_allocation = _prev_force_allocation_value;
                    __convertor->only_call = _prev_call_value;
                
                    break;

                }

            case AST_NODE_FUNCTION_CALL: 
            
                {

                    // Cause this information we be necessary for next accessing we specify with force allocation and call
                    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 1;
                    bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

                    // Makes normal execution
                    goto 
                        ast_node_function_call_execution;

                    __convertor->force_memory_allocation = _prev_force_allocation_value;
                    __convertor->only_call = _prev_call_value;

                    break;

                }

            default: throw Ordinary_Exception_Convertor("Internal error"); break;
            }
            
            break;

        default: throw Ordinary_Exception_Convertor("Internal error"); break;

    }

    // If it was accessing we need to recursively  
    if ( __ast_node_accessing->accessing->node_type == ACCESSING )

        set_byte_code_of_ast_node_accessing(
            __convertor,
            _accessing_node,
            0
        );

    // Check is is the first one meaning the accessing is done
    // And checks if there is garbage data and if is remove it
    // makes a copy of return value
    if ( __first && __convertor->current_block->current_stack_size != _stack_size_back_up ) {

        // If there was no call we need to make one here to execute the memory copy
        if ( ! __convertor->only_call ) 

            __convertor->set_byte_code_load(

                ( __convertor->force_memory_allocation )
                    ? __ast_node_accessing->representive_type->get_size() 
                    : 0

            );

        // Dealloc garbage data
        // If there was a force allocation dont deallocate that
        __convertor->set_byte_code_stack_memory_deallocation(
            __convertor->current_block->current_stack_size - _stack_size_back_up - 
                ( ( __convertor->force_memory_allocation ) ? __ast_node_accessing->representive_type->get_size() : 0 ) 
        );

        // Destination return value
        __convertor->set_byte_code_load(

            ( __convertor->force_memory_allocation )
                ? __ast_node_accessing->representive_type->get_size() 
                : 0

        );

        // Memory copy
        __convertor->set_byte_code_memory_copy(
            __ast_node_accessing->representive_type->get_size()
        );

        // If was call, need to make the call again
        if ( __convertor->only_call ) 

            __convertor->set_byte_code_load(

                ( __convertor->force_memory_allocation )
                    ? __ast_node_accessing->representive_type->get_size() 
                    : 0

            );

    }

}

void parser::set_byte_code_of_ast_node_cast(Convertor* __convertor, Ast_Node_Cast* __ast_node_cast) {

    set_byte_code_of_ast(
        __convertor,
        __ast_node_cast->constructor_call
    );

}

void parser::set_byte_code_of_ast_node_while(Convertor* __convertor, Ast_Node_While* __ast_node_while) {

    // The idea to optimize space, that all code inside the loop even tho it is run in another stack block, is to be run inside the current stack size
    // Avoiding to make the call byte code
    // So to get the effect that is is running in another stack block 
    // It will be created a new byte_code_block just for the conversion of the loop body and then will be copied to the current stack block

    // Backup current_block
    byte_code::Byte_Code_Block* _previous_block
        = __convertor->current_block;

    // Temporary byte_code_block to save the byte code of loop body
    byte_code::Byte_Code_Block* _loop_body = 
        (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_loop_body) byte_code::Byte_Code_Block(); 

    // Makes a copy into the ast node for the control_structs_key_words
    __ast_node_while->loop_body = 
        _loop_body;

    // Sets the previous current_stack value
    _loop_body->current_stack_size = 
        _previous_block->current_stack_size;

    __convertor->current_block = 
        _loop_body;

    // Struct of while loop:
    //
    //  Make the confirmation -- if is true jump specific amount of instructions, if dont jump any instructions
    //      The instructions next to confirmation is the exit loop instructions 
    //      Meaning if condition is true ( stays in the loop ) it jumps the exit instructions, if is not true dont jump the exit instructions and exit the loop
    //  
    //  Executes the body instructions
    //  At the end it returns to the begginnig of the loop and executes all again

    // Executes the condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_while->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    // Executes the confirmation
    __convertor->set_byte_code_check(1);

    // Back up for setting go to end of loop body
    int _size_byte_code_here = _loop_body->block->count;

    // Go byte code to end of loop body
    __convertor->set_byte_code_jump(0);

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_loop_body = 
        _loop_body->block->last->object;

    // Loop body
    __convertor->set_block(
        __ast_node_while->body->code
    );

    // End of body 
    // Deallocates all memory used inside the body
    // And go back to the beginnig

    // Deallocates memory
    __convertor->set_byte_code_stack_memory_deallocation(
        _loop_body->current_stack_size - _previous_block->current_stack_size
    );

    // Go back to beginning
    __convertor->set_byte_code_jump_inverted(
        _loop_body->block->count
    );

    // Now that all body is done we can set the byte code jump argument
    _copy_of_end_loop_body->argument =
        _loop_body->block->count - _size_byte_code_here;

    // Makes a copy to the previous code block
    _previous_block->block->join(
        _loop_body->block
    );

    __convertor->current_block = 
        _previous_block;

}

void parser::set_byte_code_of_ast_node_do_while(Convertor* __convertor, Ast_Node_Do_While* __ast_node_do_while) {

    // The idea to optimize space, that all code inside the loop even tho it is run in another stack block, is to be run inside the current stack size
    // Avoiding to make the call byte code
    // So to get the effect that is is running in another stack block 
    // It will be created a new byte_code_block just for the conversion of the loop body and then will be copied to the current stack block

    // Backup current_block
    byte_code::Byte_Code_Block* _previous_block
        = __convertor->current_block;

    // Temporary byte_code_block to save the byte code of loop body
    byte_code::Byte_Code_Block* _loop_body = 
        (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_loop_body) byte_code::Byte_Code_Block(); 

    // Makes a copy into the ast node for the control_structs_key_words
    __ast_node_do_while->loop_body = 
        _loop_body;

    // Sets the previous current_stack value
    _loop_body->current_stack_size = 
        _previous_block->current_stack_size;

    __convertor->current_block = 
        _loop_body;

    // Struct of while loop:
    //
    //  Executes the body instructions
    //
    //  Make the confirmation -- if is true jump specific amount of instructions, if dont jump any instructions
    //      The instructions next to confirmation is the exit loop instructions 
    //      Meaning if condition is true ( stays in the loop ) it jumps the exit instructions, if is not true dont jump the exit instructions and exit the loop
    //  
    //  At the end it returns to the begginnig of the loop and executes all again

    // Loop body
    __convertor->set_block(
        __ast_node_do_while->body->code
    );

    // Executes the condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_do_while->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    // Executes the confirmation
    __convertor->set_byte_code_check(1);

    // Back up for setting go to end of loop body
    int _size_byte_code_here = _loop_body->block->count;

    // Go byte code to end of loop body
    __convertor->set_byte_code_jump(0);

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_loop_body = 
        _loop_body->block->last->object;

    // End of body 
    // Deallocates all memory used inside the body
    // And go back to the beginnig

    // Deallocates memory
    __convertor->set_byte_code_stack_memory_deallocation(
        _loop_body->current_stack_size - _previous_block->current_stack_size
    );

    // Go back to beginning
    __convertor->set_byte_code_jump_inverted(
        _loop_body->block->count
    );

    // Now that all body is done we can set the byte code jump argument
    _copy_of_end_loop_body->argument =
        _loop_body->block->count - _size_byte_code_here;

    // Makes a copy to the previous code block
    _previous_block->block->join(
        _loop_body->block
    );

    __convertor->current_block = 
        _previous_block;

}

void parser::set_byte_code_of_ast_node_if(Convertor* __convertor, Ast_Node_If* __ast_node_if) {

    // The idea to optimize space, that all code inside the body even tho it is run in another stack block, is to be run inside the current stack size
    // Avoiding to make the call byte code
    // So to get the effect that is is running in another stack block 
    // It will be created a new byte_code_block just for the conversion of the loop body and then will be copied to the current stack block

    // Backup current_block
    byte_code::Byte_Code_Block* _previous_block
        = __convertor->current_block;

    // Temporary byte_code_block to save the byte code of loop body
    byte_code::Byte_Code_Block* _body = 
        (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_body) byte_code::Byte_Code_Block(); 

    // Sets the previous current_stack value
    _body->current_stack_size = 
        _previous_block->current_stack_size;

    __convertor->current_block = 
        _body;

    // Struct of if:
    //
    //  Make the confirmation -- if is true jump specific amount of instructions, if dont jump any instructions
    //      The instructions next to confirmation is the exit loop instructions 
    //      Meaning if condition is true ( stays in the body ) it jumps the exit instructions, if is not true dont jump the exit instructions and exit the if body
    //  
    //  Executes the body instructions
    //
    //  Go to end of all if / else if / else attached
    //

    // Executes the condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_if->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    // Executes the confirmation
    __convertor->set_byte_code_check(1);

    // Back up for setting go to end of body
    int _size_byte_code_here = _body->block->count;

    // Go byte code to end of body
    __convertor->set_byte_code_jump(0);

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_body = 
        _body->block->last->object;

    // Loop body
    __convertor->set_block(
        __ast_node_if->body->code
    );

    // End of body 
    // Deallocates all memory used inside the body

    // Deallocates memory
    __convertor->set_byte_code_stack_memory_deallocation(
        _body->current_stack_size - _previous_block->current_stack_size
    );

    // Go to end of all if statement attached
    __convertor->set_byte_code_jump(
        0
    );

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_if_statement = 
        _body->block->last->object;

    // Adds to array for at the end of all if statement it executes rigth all jumps amount
    __convertor->if_statement_go_end->add(
        _copy_of_end_if_statement
    );

    // Now that all body is done we can set the byte code jump argument
    _copy_of_end_body->argument =
        _body->block->count - _size_byte_code_here;

    // Makes a copy to the previous code block
    _previous_block->block->join(
        _body->block
    );

    // Set the current instructions length, just for help
    _copy_of_end_if_statement->argument = 
        _previous_block->block->count;

    __convertor->current_block = 
        _previous_block;

    // To explain TODO
    if ( ! __ast_node_if->next_if_statement ) {

        for (int _ = 0; _ < __convertor->if_statement_go_end->count; _++ )

            __convertor->if_statement_go_end->operator[](_)->argument = 
                __convertor->current_block->block->count - __convertor->if_statement_go_end->operator[](_)->argument + 1;

        __convertor->if_statement_go_end->clean();

    }

}

void parser::set_byte_code_of_ast_node_else_if(Convertor* __convertor, Ast_Node_Else_If* __ast_node_else_if) {

    // The idea to optimize space, that all code inside the body even tho it is run in another stack block, is to be run inside the current stack size
    // Avoiding to make the call byte code
    // So to get the effect that is is running in another stack block 
    // It will be created a new byte_code_block just for the conversion of the loop body and then will be copied to the current stack block

    // Backup current_block
    byte_code::Byte_Code_Block* _previous_block
        = __convertor->current_block;

    // Temporary byte_code_block to save the byte code of loop body
    byte_code::Byte_Code_Block* _body = 
        (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_body) byte_code::Byte_Code_Block(); 

    // Sets the previous current_stack value
    _body->current_stack_size = 
        _previous_block->current_stack_size;

    __convertor->current_block = 
        _body;

    // Struct of else if:
    //
    //  Make the confirmation -- if is true jump specific amount of instructions, if dont jump any instructions
    //      The instructions next to confirmation is the exit loop instructions 
    //      Meaning if condition is true ( stays in the body ) it jumps the exit instructions, if is not true dont jump the exit instructions and exit the if body
    //  
    //  Executes the body instructions
    //
    //  Go to end of all if / else if / else attached
    //

    // Executes the condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_else_if->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    // Executes the confirmation
    __convertor->set_byte_code_check(1);

    // Back up for setting go to end of body
    int _size_byte_code_here = _body->block->count;

    // Go byte code to end of body
    __convertor->set_byte_code_jump(0);

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_body = 
        _body->block->last->object;

    // Loop body
    __convertor->set_block(
        __ast_node_else_if->body->code
    );

    // End of body 
    // Deallocates all memory used inside the body

    // Deallocates memory
    __convertor->set_byte_code_stack_memory_deallocation(
        _body->current_stack_size - _previous_block->current_stack_size
    );

    // Go to end of all if statement attached
    __convertor->set_byte_code_jump(
        0
    );

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_if_statement = 
        _body->block->last->object;

    // Adds to array for at the end of all if statement it executes rigth all jumps amount
    __convertor->if_statement_go_end->add(
        _copy_of_end_if_statement
    );

    // Now that all body is done we can set the byte code jump argument
    _copy_of_end_body->argument =
        _body->block->count - _size_byte_code_here;

    // Makes a copy to the previous code block
    _previous_block->block->join(
        _body->block
    );

    // Set the current instructions length, just for help
    _copy_of_end_if_statement->argument = 
        _previous_block->block->count;

    __convertor->current_block = 
        _previous_block;

    // To explain TODO
    if ( ! __ast_node_else_if->next_if_statement ) {

        for (int _ = 0; _ < __convertor->if_statement_go_end->count; _++ )

            __convertor->if_statement_go_end->operator[](_)->argument = 
                __convertor->current_block->block->count - __convertor->if_statement_go_end->operator[](_)->argument + 1;

        __convertor->if_statement_go_end->clean();

    }

}

void parser::set_byte_code_of_ast_node_else(Convertor* __convertor, Ast_Node_Else* __ast_node_else) {

    // The idea to optimize space, that all code inside the body even tho it is run in another stack block, is to be run inside the current stack size
    // Avoiding to make the call byte code
    // So to get the effect that is is running in another stack block 
    // It will be created a new byte_code_block just for the conversion of the loop body and then will be copied to the current stack block

    // Backup current_block
    byte_code::Byte_Code_Block* _previous_block
        = __convertor->current_block;

    // Temporary byte_code_block to save the byte code of loop body
    byte_code::Byte_Code_Block* _body = 
        (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_body) byte_code::Byte_Code_Block(); 

    // Sets the previous current_stack value
    _body->current_stack_size = 
        _previous_block->current_stack_size;

    __convertor->current_block = 
        _body;

    // Struct of else:
    //
    //  Executes the body instructions
    //
    //  Go to end of all if / else if / else attached
    //

    // Loop body
    __convertor->set_block(
        __ast_node_else->body->code
    );

    // End of body 
    // Deallocates all memory used inside the body

    // Deallocates memory
    __convertor->set_byte_code_stack_memory_deallocation(
        _body->current_stack_size - _previous_block->current_stack_size
    );

    // Go to end of all if statement attached
    __convertor->set_byte_code_jump(
        0
    );

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_if_statement = 
        _body->block->last->object;

    // Adds to array for at the end of all if statement it executes rigth all jumps amount
    __convertor->if_statement_go_end->add(
        _copy_of_end_if_statement
    );

    // Makes a copy to the previous code block
    _previous_block->block->join(
        _body->block
    );

    // Set the current instructions length, just for help
    _copy_of_end_if_statement->argument = 
        _previous_block->block->count;

    __convertor->current_block = 
        _previous_block;

    // To explain TODO

    for (int _ = 0; _ < __convertor->if_statement_go_end->count; _++ )

        __convertor->if_statement_go_end->operator[](_)->argument = 
            __convertor->current_block->block->count - __convertor->if_statement_go_end->operator[](_)->argument + 1;

    __convertor->if_statement_go_end->clean();


}

void parser::set_byte_code_of_ast_node_for(Convertor* __convertor, Ast_Node_For* __ast_node_for) {

    // The idea to optimize space, that all code inside the loop even tho it is run in another stack block, is to be run inside the current stack size
    // Avoiding to make the call byte code
    // So to get the effect that is is running in another stack block 
    // It will be created a new byte_code_block just for the conversion of the loop body and then will be copied to the current stack block

    // Backup current_block
    byte_code::Byte_Code_Block* _previous_block
        = __convertor->current_block;

    // Temporary byte_code_block to save the byte code of loop body
    byte_code::Byte_Code_Block* _loop_body = 
        (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

    new (_loop_body) byte_code::Byte_Code_Block(); 

    // Makes a copy into the ast node for the control_structs_key_words
    __ast_node_for->loop_body = 
        _loop_body;

    // Sets the previous current_stack value
    _loop_body->current_stack_size = 
        _previous_block->current_stack_size;

    __convertor->current_block = 
        _loop_body;


    // Struct of while loop:
    //
    //  Initialize the given instructions
    //
    //  Make the confirmation -- if is true jump specific amount of instructions, if dont jump any instructions
    //      The instructions next to confirmation is the exit loop instructions 
    //      Meaning if condition is true ( stays in the loop ) it jumps the exit instructions, if is not true dont jump the exit instructions and exit the loop
    //      In for loop we have to deallocates all memory inialized in iniatial for   
    //
    //  Executes the body instructions
    //  
    //  Execute the "execution" instructions
    //
    //  At the end it returns to the begginnig of the loop after initialized variables and executes all again

    // Initialize all the variables
    for ( int _ = 0; _ < __ast_node_for->variable_declarations->count; _++ )

        set_byte_code_of_ast(
            __convertor,
            __ast_node_for->variable_declarations->operator[](_)
        );

    // Get the current size so when the deallocation is made we dont deallocate the variables initialized
    int _stack_size_after_variables_initialization = 
        _loop_body->current_stack_size;

    // Get the byte code length needed for the variables declarations so when loop repeats we dont initialize all the variables again
    int _byte_code_length_after_variables_initialization = 
        _loop_body->block->count;

    // Executes the condition
    bool _prev_call = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_for->condition->constructor_call
    );

    __convertor->only_call = _prev_call;

    // Executes the confirmation
    __convertor->set_byte_code_check(2);

    // Deallocates for inialization variables
    __convertor->set_byte_code_stack_memory_deallocation(
        _stack_size_after_variables_initialization - _previous_block->current_stack_size
    );

    // Back up for setting go to end of loop body
    int _size_byte_code_here = _loop_body->block->count;

    // Back to add all memory deallocated cause of function above
    _loop_body->current_stack_size += 
        _stack_size_after_variables_initialization - _previous_block->current_stack_size;

    // Go byte code to end of loop body
    __convertor->set_byte_code_jump(0);

    // Beacause at this time we dont know what is the full amount of instructions
    // Byte code to take a copy 
    byte_code::Byte_Code* _copy_of_end_loop_body = 
        _loop_body->block->last->object;

    // Loop body
    __convertor->set_block(
        __ast_node_for->body->code
    );

    // Executes the "execution" instructions 

    for ( int _ = 0; _ < __ast_node_for->execution->count; _++ )

        set_byte_code_of_ast(
            __convertor,
            __ast_node_for->execution->operator[](_)
        );

    // End of body 
    // Deallocates all memory used inside the body except for the variables initialized
    // And go back to the beginnig

    // Deallocates memory
    __convertor->set_byte_code_stack_memory_deallocation(
        _loop_body->current_stack_size - _stack_size_after_variables_initialization
    );

    // Go back to beginning after variables initialization
    __convertor->set_byte_code_jump_inverted(
        _loop_body->block->count - _byte_code_length_after_variables_initialization
    );

    // Now that all body is done we can set the byte code jump argument
    _copy_of_end_loop_body->argument =
        _loop_body->block->count - _size_byte_code_here;

    // Makes a copy to the previous code block
    _previous_block->block->join(
        _loop_body->block
    );

    __convertor->current_block = 
        _previous_block;

}

void parser::set_byte_code_of_ast_node_control_structs_key_words(Convertor* __convertor, Ast_Node_Control_Structs_Key_Words* __ast_node_control_structs_key_words) {

    // First we get the loop body of the control struct 
    // Then:
    //
    //  For the "break" instruction:
    //
    //  For the "continue" instruction:
    //
    //

    byte_code::Byte_Code_Block* _loop_body;

    // Getting the loop body
    switch (__ast_node_control_structs_key_words->control_struct->node_type)
    {
        // Even tho this are different structs they share the same parameters, so there is no problem in casting both into Ast_Node_While
        case AST_NODE_WHILE: case AST_NODE_DO_WHILE:

            _loop_body = 
                ( (Ast_Node_While*) __ast_node_control_structs_key_words->control_struct )->loop_body; break;

        case AST_NODE_FOR:

            _loop_body = 
            ( (Ast_Node_For*) __ast_node_control_structs_key_words->control_struct)->loop_body; break;

        default: throw Ordinary_Exception_Convertor("Internal error"); break;
    }

    // instructions execution

    switch (__ast_node_control_structs_key_words->key_word_id)
    {
        case BREAK:

            break;
        case CONTINUE:

            break;

        default: throw Ordinary_Exception_Convertor("Internal error"); break;
    }

}

void parser::set_byte_code_of_ast_type_conversion(Convertor* __convertor, Ast_Node_Type_Conversion* __ast_node_type_conversion) {

    // Here is just the type conversion 
    // So it just s executes the value
    // And force memory or call according to force_memory_allocation and only_call

    bool _prev_force_allocation_value = __convertor->force_memory_allocation; __convertor->force_memory_allocation = 0;
    // bool _prev_call_value = __convertor->only_call; __convertor->only_call = 1;

    set_byte_code_of_ast(
        __convertor,
        __ast_node_type_conversion->value
    );

    __convertor->force_memory_allocation = _prev_force_allocation_value;
    // __convertor->only_call = _prev_call_value;

    if ( __convertor->force_memory_allocation )

        __convertor->set_byte_code_stack_memory_allocation(
            __ast_node_type_conversion->representive_type->get_size()
        );

}

