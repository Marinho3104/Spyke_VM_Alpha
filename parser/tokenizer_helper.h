#pragma once

#ifndef PARSER_TOKENIZER_HELPER
#define PARSER_TOKENIZER_HELPER

#include "parser_definitions.h"

namespace parser {

    int get_token_symbol_id(const char*&);

    int get_token_keyword_id(const char*&, int);

    /* Implicit Values */

    bool only_digist(char*);

    long get_long_from_string(char*, Tokenizer*);

    bool is_int(char*, Tokenizer*);

}

#endif