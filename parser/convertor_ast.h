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

    void set_byte_code_of_ast_node_parenthesis(Convertor*, Ast_Node_Parenthesis*);

    void set_byte_code_of_ast_node_byte_code(Convertor*, Ast_Node_Byte_Code*);

    void set_byte_code_of_ast_node_return_key_word(Convertor*, Ast_Node_Return_Key_Word*);

    void set_byte_code_of_ast_node_constructor_call(Convertor*, Ast_Node_Constructor_Call*);

    void set_byte_code_of_ast_node_accessing(Convertor*, Ast_Node_Accessing*, bool);

    void set_byte_code_of_ast_node_cast(Convertor*, Ast_Node_Cast*);

    void set_byte_code_of_ast_node_while(Convertor*, Ast_Node_While*);

    void set_byte_code_of_ast_node_do_while(Convertor*, Ast_Node_Do_While*);

    void set_byte_code_of_ast_node_if(Convertor*, Ast_Node_If*);

    void set_byte_code_of_ast_node_else_if(Convertor*, Ast_Node_Else_If*);
    
    void set_byte_code_of_ast_node_else(Convertor*, Ast_Node_Else*);

    void set_byte_code_of_ast_node_control_structs_key_words(Convertor*, Ast_Node_Control_Structs_Key_Words*);

    void set_byte_code_of_ast_node_for(Convertor*, Ast_Node_For*);

}

#endif