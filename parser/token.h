#pragma once

#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

#include <stddef.h>

namespace parser {

    struct Position_Information {

        size_t column, line, size;

        Position_Information(size_t, size_t, size_t);

    };

    struct Token {

        int id;
        char* identifier;
        Position_Information position_information;

        ~Token(); Token(int, char*, size_t, size_t, size_t); Token(int, char*, Position_Information);

        void print();

        Token* get_copy();

    };

    bool is_primitive_type(int);

}

#endif