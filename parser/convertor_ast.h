#pragma once

#ifndef CONVERTOR_AST_H
#define CONVERTOR_AST_H

#include "parser_definitions.h"

namespace parser {

    void set_byte_code_of_ast(Convertor*, Ast_Node*);

    void set_byte_code_of_ast_node_name_space(Convertor*, Ast_Node_Name_Space*);

    void set_byte_code_of_ast_node_code_block(Convertor*, Ast_Node_Code_Block*);

    void set_byte_code_of_ast_node_struct_declaration(Convertor*, Ast_Node_Struct_Declaration*);
    
    void set_byte_code_of_ast_node_function_declaration(Convertor*, Ast_Node_Function_Declaration*);

    void set_byte_code_of_ast_node_variable_declaration(Convertor*, Ast_Node_Variable_Declaration*);

    void set_byte_code_of_ast_node_expression(Convertor*, Ast_Node_Expression*);

    void set_byte_code_of_ast_node_variable(Convertor*, Ast_Node_Variable*);

    void set_byte_code_of_ast_node_implicit_value(Convertor*, Ast_Node_Implicit_Value*);

    void set_byte_code_of_ast_node_function_call(Convertor*, Ast_Node_Function_Call*);

    void set_byte_code_of_ast_node_pointer_operations(Convertor*, Ast_Node_Pointer_Operation*);

    utils::Linked_List <byte_code::Byte_Code*>* set_byte_code_of_ast_node_parenthesis(Convertor*, Ast_Node_Parenthesis*);

    utils::Linked_List <byte_code::Byte_Code*>* set_byte_code_of_ast_node_constructor_call(Convertor*, Ast_Node_Constructor_Call*);

    utils::Linked_List <byte_code::Byte_Code*>* set_byte_code_of_ast_node_accessing(Convertor*, Ast_Node_Accessing*);

    utils::Linked_List <byte_code::Byte_Code*>* set_byte_code_of_ast_node_byte_code(Convertor*, Ast_Node_Byte_Code*);

    utils::Linked_List <byte_code::Byte_Code*>* set_byte_code_of_ast_node_cast(Convertor*, Ast_Node_Cast*);

    utils::Linked_List <byte_code::Byte_Code*>* set_byte_code_of_ast_node_return_key_word(Convertor*, Ast_Node_Return_Key_Word*);

    byte_code::Byte_Code* set_byte_code_of_ast_node_while(Convertor*, Ast_Node_While*);

}

#endif