#pragma once

#ifndef CONVERTOR_AST_H
#define CONVERTOR_AST_H

#include "parser_definitions.h"

namespace parser {

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast(Convertor*, Ast_Node*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_name_space(Convertor*, Ast_Node_Name_Space*);

    void get_byte_code_of_ast_node_code_block(Convertor*, Ast_Node_Code_Block*);

    void get_byte_code_of_ast_node_struct_declaration(Convertor*, Ast_Node_Struct_Declaration*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_variable_declaration(Convertor*, Ast_Node_Variable_Declaration*);
    
    void get_byte_code_of_ast_node_function_declaration(Convertor*, Ast_Node_Function_Declaration*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_expression(Convertor*, Ast_Node_Expression*);

    byte_code::Byte_Code* get_byte_code_of_ast_node_variable(Convertor*, Ast_Node_Variable*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_function_call(Convertor*, Ast_Node_Function_Call*);

    byte_code::Byte_Code* get_byte_code_of_ast_node_implicit_value(Convertor*, Ast_Node_Implicit_Value*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_pointer_operations(Convertor*, Ast_Node_Pointer_Operation*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_parenthesis(Convertor*, Ast_Node_Parenthesis*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_constructor_call(Convertor*, Ast_Node_Constructor_Call*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_accessing(Convertor*, Ast_Node_Accessing*);

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_ast_node_byte_code(Convertor*, Ast_Node_Accessing*);

}

#endif