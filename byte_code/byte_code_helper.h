#pragma once

#ifndef BYTE_CODE_HELPER_H
#define BYTE_CODE_HELPER_H

#include "byte_code_definitions.h"

namespace byte_code {

    void set_implicit_value_content(void*, parser::Token*);

    void set_implicit_int_content(void*, parser::Token*);

}

#endif