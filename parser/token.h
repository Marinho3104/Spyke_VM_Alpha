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

        ~Token(); Token(int, char*); Token(int, char*, size_t, size_t, size_t); Token(int, char*, Position_Information);

        bool operator==(Token*);

        void print();

        Token* get_copy();

    };

    bool is_primitive_type(int);

    bool is_implicit_value_type(int);

    bool is_single_parameters_function_operation(int);

    int get_primitive_type_of_implicit_value_type(int);

}

#endif