#include "byte_code_helper.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "token.h"

#include <iostream>



void byte_code::set_implicit_value_content(void* __address, parser::Token* __token) {

    switch (__token->id)
    {
    case IMPLICIT_VALUE_INT: set_implicit_int_content(__address, __token); break;
    default: break;
    }

}

void byte_code::set_implicit_int_content(void* __address, parser::Token* __token) 
    { *( (int*) __address ) = atoi(__token->identifier); }



