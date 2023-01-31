#include "built_ins.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "common.h"

#include <iostream>

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
