#pragma once 

#ifndef PARSER_PRE_COMPILER_WARNINGS_H
#define PARSER_PRE_COMPILER_WARNINGS_H

#include "parser_definitions.h"

namespace parser {

    struct More_Arguments_Than_Needed_Warning {

        static void launch(Code_Information*, int, int, int);

    };

}

#endif