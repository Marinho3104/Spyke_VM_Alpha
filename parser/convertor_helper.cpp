#include "convertor_helper.h"

#include "convertor_ast.h"
#include "linked_list.h"
#include "byte_code.h"
#include "convertor.h"
#include "ast_nodes.h"
#include "opcodes.h"

#include <iostream>

utils::Linked_List <byte_code::Byte_Code*>* parser::get_byte_code_of_condition(Convertor* __convertor, Ast_Node_Variable_Declaration* __condition) {

    // utils::Linked_List <byte_code::Byte_Code*>* _byte_code =
    //     new utils::Linked_List <byte_code::Byte_Code*>(0);

    // __condition->stack_position = 
    //     __convertor->current_block->current_stack_size;

    // _byte_code->add(
    //     byte_code::Byte_Code::generate(
    //         STACK_MEMORY_ALLOCATION,
    //         1
    //     )
    // );

    // __convertor->current_block->current_stack_size ++;

    // utils::Linked_List <byte_code::Byte_Code*>* _temp =
    //     get_byte_code_of_ast_node_function_call(
    //         __convertor,
    //         __condition->constructor_call->constructor
    //     );

    // _byte_code->join(
    //     _temp
    // );

    // delete _temp;

    // _byte_code->add(
    //     byte_code::Byte_Code::generate(
    //         STACK_MEMORY_DEALLOCATION,
    //         1
    //     )
    // );

    // __convertor->current_block->current_stack_size --;

 
    // return _byte_code;

}


