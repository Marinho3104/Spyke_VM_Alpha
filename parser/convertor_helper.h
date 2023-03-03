#pragma once

#ifndef CONVERTOR_HELPER_H
#define CONVERTOR_HELPER_H

#include "parser_definitions.h"

namespace parser {

    utils::Linked_List <byte_code::Byte_Code*>* get_byte_code_of_condition(Convertor*, Ast_Node_Variable_Declaration*);

}

#endif