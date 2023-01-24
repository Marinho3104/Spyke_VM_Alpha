#include "tokenizer_helper.h"

#include "token_definitions.h"

#include <string.h>
#include <iostream>

int parser::get_token_symbol_id(const char*& __current_position) {

    if (!strncmp(__current_position, "<<=", 3)) { __current_position += 3; return FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT_ASSIGN; }
    if (!strncmp(__current_position, ">>=", 3)) { __current_position += 3; return FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT_ASSIGN; }

    if (!strncmp(__current_position, "::", 2)) { __current_position += 2; return DOUBLE_COLON; }
    if (!strncmp(__current_position, "//", 2)) { __current_position += 2; return COMMENT_SINGLE_LINE; }
    if (!strncmp(__current_position, "/*", 2)) { __current_position += 2; return COMMENT; }
    if (!strncmp(__current_position, "*/", 2)) { __current_position += 2; return COMMENT_END; }

    if (!strncmp(__current_position, "+=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_PLUS_ASSIGN; }
    if (!strncmp(__current_position, "-=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_MINUS_ASSIGN; }
    if (!strncmp(__current_position, "*=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_MULTIPLICATION_ASSIGN; }
    if (!strncmp(__current_position, "/=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_DIVISION_ASSIGN; }
    if (!strncmp(__current_position, "%=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_MODULOS_ASSIGN; }

    if (!strncmp(__current_position, "++", 2)) { __current_position += 2; return FUNCTION_OPERATOR_INCREMENT; }
    if (!strncmp(__current_position, "--", 2)) { __current_position += 2; return FUNCTION_OPERATOR_DECREMENT; }

    if (!strncmp(__current_position, "&&", 2)) { __current_position += 2; return FUNCTION_OPERATOR_AND; }
    if (!strncmp(__current_position, "||", 2)) { __current_position += 2; return FUNCTION_OPERATOR_OR; }

    if (!strncmp(__current_position, "==", 2)) { __current_position += 2; return FUNCTION_OPERATOR_EQUAL_TO; }
    if (!strncmp(__current_position, "!=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_NOT_EQUAL_TO; }
    if (!strncmp(__current_position, ">=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_GREATER_THAN_EQUAL_TO; }
    if (!strncmp(__current_position, "<=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO; }

    if (!strncmp(__current_position, "<<", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT; }
    if (!strncmp(__current_position, ">>", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT; }

    if (!strncmp(__current_position, "&=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_AND_ASSIGN; }
    if (!strncmp(__current_position, "|=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_OR_ASSIGN; }
    if (!strncmp(__current_position, "^=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_XOR_ASSIGN; }

    if (!strncmp(__current_position, "b+", 2)) { __current_position += 2; return BINARY_ADDITION; }
    if (!strncmp(__current_position, "b-", 2)) { __current_position += 2; return BINARY_SUBTRACTION; }
    if (!strncmp(__current_position, "b*", 2)) { __current_position += 2; return BINARY_MULTIPLICATION; }
    if (!strncmp(__current_position, "b/", 2)) { __current_position += 2; return BINARY_DIVISION; }
    if (!strncmp(__current_position, "b%", 2)) { __current_position += 2; return BINARY_MODULOS; }

    if (!strncmp(__current_position, "->", 2)) { __current_position += 2; return ACCESSING_POINTER; }

    if (!strncmp(__current_position, ";", 1)) { __current_position += 1; return END_INSTRUCTION; }

    if (!strncmp(__current_position, "(", 1)) { __current_position += 1; return OPEN_PARENTHESIS; }
    if (!strncmp(__current_position, ")", 1)) { __current_position += 1; return CLOSE_PARENTHESIS; }

    if (!strncmp(__current_position, "{", 1)) { __current_position += 1; return OPEN_BRACES; }
    if (!strncmp(__current_position, "}", 1)) { __current_position += 1; return CLOSE_BRACES; }

    if (!strncmp(__current_position, "[", 1)) { __current_position += 1; return OPEN_BRACKET; }
    if (!strncmp(__current_position, "]", 1)) { __current_position += 1; return CLOSE_BRACKET; }

    if (!strncmp(__current_position, ",", 1)) { __current_position += 1; return COMMA; }
    if (!strncmp(__current_position, ":", 1)) { __current_position += 1; return COLON; }
    if (!strncmp(__current_position, "?", 1)) { __current_position += 1; return QUESTION_MARK; }
    if (!strncmp(__current_position, "\"", 1)) { __current_position += 1; return QUOTATION_MARK; }
    if (!strncmp(__current_position, "'", 1)) { __current_position += 1; return SINGLE_QUOTATION_MARK; }
    if (!strncmp(__current_position, "#", 1)) { __current_position += 1; return PRE_PROCESSOR; }

    if (!strncmp(__current_position, "*", 1)) { __current_position += 1; return POINTER; }
    if (!strncmp(__current_position, "&", 1)) { __current_position += 1; return ADDRESS; }

    if (!strncmp(__current_position, "+", 1)) { __current_position++; return FUNCTION_OPERATOR_PLUS; }
    if (!strncmp(__current_position, "-", 1)) { __current_position++; return FUNCTION_OPERATOR_MINUS; }
    // if (!strncmp(__current_position, "*", 1)) { __current_position++; return FUNCTION_OPERATOR_MULTIPLICATION; } // Never reatch
    if (!strncmp(__current_position, "/", 1)) { __current_position++; return FUNCTION_OPERATOR_DIVISION; }
    if (!strncmp(__current_position, "%", 1)) { __current_position++; return FUNCTION_OPERATOR_MODULOS; }

    if (!strncmp(__current_position, "=", 1)) { __current_position++; return FUNCTION_OPERATOR_EQUAL; }

    if (!strncmp(__current_position, "!", 1)) { __current_position++; return FUNCTION_OPERATOR_NOT; }

    if (!strncmp(__current_position, ">", 1)) { __current_position += 1; return FUNCTION_OPERATOR_GREATER_THAN; }
    if (!strncmp(__current_position, "<", 1)) { __current_position += 1; return FUNCTION_OPERATOR_LESS_THAN; }

    if (!strncmp(__current_position, "~", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_NOT; }

    // if (!strncmp(__current_position, "&", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_AND; } // Never reatch
    if (!strncmp(__current_position, "|", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_OR; }
    if (!strncmp(__current_position, "^", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_XOR; }

    if (!strncmp(__current_position, ".", 1)) { __current_position += 1; return ACCESSING; }

    return -1;
    
}
