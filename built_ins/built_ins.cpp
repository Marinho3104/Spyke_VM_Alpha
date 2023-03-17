#include "built_ins.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "common.h"

#include <iostream>
#include <string.h>

char* built_ins::get_struct_name_of_primitive_type(int __token_id) {

    char* _struct_name = 0;

    switch (__token_id)
    {
        case PRIMITIVE_TYPE_VOID: _struct_name = utils::get_string_copy( (char*) PRIMITIVE_TYPE_VOID_NAME ); break;
        case PRIMITIVE_TYPE_BYTE: _struct_name = utils::get_string_copy( (char*) PRIMITIVE_TYPE_BYTE_NAME ); break;
        case PRIMITIVE_TYPE_INT: _struct_name = utils::get_string_copy( (char*) PRIMITIVE_TYPE_INT_NAME ); break;
        case PRIMITIVE_TYPE_BOOL: _struct_name = utils::get_string_copy( (char*) PRIMITIVE_TYPE_BOOL_NAME ); break;
        case PRIMITIVE_TYPE_SPYKE: _struct_name = utils::get_string_copy( (char*) PRIMITIVE_TYPE_SPYKE_NAME ); break;
        case PRIMITIVE_TYPE_POINTER: _struct_name = utils::get_string_copy( (char*) PRIMITIVE_TYPE_POINTER_NAME ); break; 
        default: break;
    }

    return _struct_name;

}

char* built_ins::get_struct_function_name_of_operation_id(int __operation_id) {

    char* _function_name = 0;

    switch (__operation_id)
    {
    case FUNCTION_OPERATOR_PLUS: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_PLUS_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_MINUS: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_MINUS_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_MULTIPLICATION: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_MULTIPLICATION_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_DIVISION: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_DIVISION_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_MODULOS: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_MODULOS_FUNCTION_NAME ); break;

    case FUNCTION_OPERATOR_EQUAL: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_EQUAL_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_PLUS_ASSIGN: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_PLUS_ASSIGN_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_MINUS_ASSIGN: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_MINUS_ASSIGN_FUNCTION_NAME ); break;

    case FUNCTION_OPERATOR_INCREMENT: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_INCREMENT_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_DECREMENT: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_DECREMENT_FUNCTION_NAME ); break;

    case FUNCTION_OPERATOR_INCREMENT_LEFT: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_INCREMENT_LEFT_FUNCTION_NAME ); break;
    case FUNCTION_OPERATOR_DECREMENT_LEFT: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_DECREMENT_LEFT_FUNCTION_NAME ); break;

    case FUNCTION_OPERATOR_BRACKET: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_BRACKET_FUNCTION_NAME ); break;

    case FUNCTION_OPERATOR_NOT: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_NOT_FUNCTION_NAME ); break;

    case FUNCTION_OPERATOR_EQUAL_TO: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_EQUAL_TO_NAME ); break;
    case FUNCTION_OPERATOR_NOT_EQUAL_TO: _function_name = utils::get_string_copy( (char*) FUNCTION_OPERATOR_NOT_EQUAL_TO_NAME ); break;
    
    default: break;
    }

    return _function_name;

}

int built_ins::get_primitive_type_size(char* __type_name) {

    if (!strcmp(PRIMITIVE_TYPE_VOID_NAME, __type_name)) return PRIMITIVE_TYPE_VOID_SIZE;
    if (!strcmp(PRIMITIVE_TYPE_BYTE_NAME, __type_name)) return PRIMITIVE_TYPE_BYTE_SIZE;
    if (!strcmp(PRIMITIVE_TYPE_INT_NAME, __type_name)) return PRIMITIVE_TYPE_INT_SIZE;
    if (!strcmp(PRIMITIVE_TYPE_BOOL_NAME, __type_name)) return PRIMITIVE_TYPE_BOOL_SIZE;
    if (!strcmp(PRIMITIVE_TYPE_SPYKE_NAME, __type_name)) return PRIMITIVE_TYPE_SPYKE_SIZE;
    if (!strcmp(PRIMITIVE_TYPE_POINTER_NAME, __type_name)) return PRIMITIVE_TYPE_POINTER_SIZE;

    return -1;

}





