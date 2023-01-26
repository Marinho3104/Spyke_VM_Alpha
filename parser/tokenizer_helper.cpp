#include "tokenizer_helper.h"

#include "tokenizer_exceptions.h"
#include "token_definitions.h"

#include <string.h>
#include <limits.h>
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
    if (!strncmp(__current_position, "#", 1)) { __current_position += 1; return PRE_COMPILER; }

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

int parser::get_token_keyword_id(const char*& __current_position, int __last_toke_id) {

    if (!strncmp(__current_position, "void", 4)) { __current_position += 4; return PRIMITIVE_TYPE_VOID; }
    if (!strncmp(__current_position, "byte", 4)) { __current_position += 4; return PRIMITIVE_TYPE_BYTE; }
    if (!strncmp(__current_position, "int", 3)) { __current_position += 3; return PRIMITIVE_TYPE_INT; }
    if (!strncmp(__current_position, "bool", 4)) { __current_position += 4; return PRIMITIVE_TYPE_BOOL; }
    if (!strncmp(__current_position, "pointer", 7)) { __current_position += 7; return PRIMITIVE_TYPE_POINTER; }
    if (!strncmp(__current_position, "spyke", 5)) { __current_position += 5; return PRIMITIVE_TYPE_SPYKE; }
    
    if (!strncmp(__current_position, "namespace", 9)) { __current_position += 9; return NAMESPACE; }
    if (!strncmp(__current_position, "struct", 6)) { __current_position += 6; return STRUCT; }
    if (!strncmp(__current_position, "contract", 8)) { __current_position += 8; return CONTRACT; }
    if (!strncmp(__current_position, "static", 6)) { __current_position += 6; return STATIC; }
    if (!strncmp(__current_position, "exec", 4)) { __current_position += 4; return EXEC; }
    if (!strncmp(__current_position, "return", 6)) { __current_position += 6; return RETURN; }

    if (__last_toke_id == PRE_COMPILER && !strncmp(__current_position, "define", 6)) { __current_position += 6; return PRE_COMPILER_DEFINE; }
    if (__last_toke_id == PRE_COMPILER && !strncmp(__current_position, "include", 7)) { __current_position += 7; return PRE_COMPILER_INCLUDE; }
    if (__last_toke_id == PRE_COMPILER && !strncmp(__current_position, "ifdef", 5)) { __current_position += 5; return PRE_COMPILER_IF_DEF; }
    if (__last_toke_id == PRE_COMPILER && !strncmp(__current_position, "ifndef", 6)) { __current_position += 6; return PRE_COMPILER_IF_N_DEF; }
    if (__last_toke_id == PRE_COMPILER && !strncmp(__current_position, "else", 4)) { __current_position += 4; return PRE_COMPILER_ELSE; }
    if (__last_toke_id == PRE_COMPILER && !strncmp(__current_position, "endif", 5)) { __current_position += 5; return PRE_COMPILER_END_IF; }

    if (!strncmp(__current_position, "if", 2)) { __current_position += 2; return IF; }
    if (!strncmp(__current_position, "else if", 7)) { __current_position += 7; return ELSE_IF; }
    if (!strncmp(__current_position, "else", 4)) { __current_position += 4; return ELSE; }
    if (!strncmp(__current_position, "sizeof", 6)) { __current_position += 6; return SIZE_OF; }
    if (!strncmp(__current_position, "while", 5)) { __current_position += 5; return WHILE; }
    if (!strncmp(__current_position, "do", 2)) { __current_position += 2; return DO; }
    if (!strncmp(__current_position, "break", 5)) { __current_position += 5; return BREAK; }
    if (!strncmp(__current_position, "continue", 8)) { __current_position += 8; return CONTINUE; }

    return -1;

}


bool parser::only_digist(char* __str) {

    while(*__str) {

        if ( (*__str < 48 || *__str > 57) && *__str != 95 ) return 0;

        __str++;

    }

    return 1;

}

long parser::get_long_from_string(char* __str, Tokenizer* __tokenizer) {

    long _value = 0;
    char* _end_p;

    _value = strtol(__str, &_end_p, 10);

    if (_value == 0 && errno == EINVAL) exit(1); // Throw

    else if ( (_value == LONG_MIN || _value == LONG_MAX) && errno == ERANGE ) 

        throw Implicit_Value_Bad_Structure_Exception_Tokenizer(__tokenizer);

    return _value;

}

bool parser::is_int(char* __is_int, Tokenizer* __tokenizer) { return only_digist(__is_int) && get_long_from_string(__is_int, __tokenizer) <= INT_MAX; }





