

#define AST_NODE_NAME_SPACE 0
#define AST_NODE_CODE_BLOCK 1
#define AST_NODE_STRUCT_DECLARATION 2
#define AST_NODE_VARIABLE_DECLARATION 3
#define AST_NODE_FUNCTION_DECLARATION 4
#define AST_NODE_EXPRESSION 5
#define AST_NODE_VARIABLE 6
#define AST_NODE_FUNCTION_CALL 7
#define AST_NODE_IMPLICIT_VALUE 8
#define AST_NODE_POINTER_OPERATION 9
#define AST_NODE_PARENTHESIS 10
#define AST_NODE_ACCESSING 11
#define AST_NODE_BYTE_CODE 12
#define AST_NODE_CONSTRUCTOR_CALL 13
#define AST_NODE_CAST 14
#define AST_NODE_RETURN_KEY_WORD 15
#define AST_NODE_WHILE 16
#define AST_NODE_DO_WHILE 17
#define AST_NODE_IF 18
#define AST_NODE_ELSE_IF 19
#define AST_NODE_ELSE 20
#define AST_NODE_FOR 21
#define AST_NODE_CONTROL_STRUCTS_KEY_WORD 22
#define AST_NODE_CONTRACT_DECLARATION 23
#define AST_NODE_TYPE_CONVERSION 24

/* Ast Helper Definitions for get_node_type() */

#define AST_CLOSE_BRACE -1 // }
#define AST_END_INSTRUCTION -2 // ;
#define AST_CLOSE_PARENTHESIS -3 // )
#define AST_BITWISE_NOT -4 // ~


/* Name Space Type */

#define NAME_SPACE_TYPE_NAME_SPACE 0
#define NAME_SPACE_TYPE_CODE_BLOCK 1
#define NAME_SPACE_TYPE_STRUCT_BODY 2
#define NAME_SPACE_TYPE_FUNCTION_BODY 3

