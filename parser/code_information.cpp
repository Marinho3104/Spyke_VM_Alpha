#include "code_information.h"

#include "code_information_helper.h"
#include "linked_list.h"
#include "common.h"

#include <iostream>
#include <string.h>

parser::Code_Information::~Code_Information() { free(code); delete code_by_lines; }

parser::Code_Information::Code_Information(char* __code) : code(__code) 
    { code_by_lines = new utils::Linked_List <char*>(); set_code_by_lines(); }

void parser::Code_Information::set_code_by_lines() {
    
    char* _splitted_string, *_code_copy = code;
    size_t _pos;

    while(*_code_copy) {

        _pos = get_character_position(_code_copy, '\n');

        _splitted_string = utils::get_string_copy_n(
            _code_copy,
            _pos
        );

        _code_copy += _pos;

        if (*_code_copy == '\n') ++_code_copy;

        code_by_lines->add(
            _splitted_string
        );

    }

}


utils::Linked_List <char*>* parser::Code_Information::get_line_preview(int __line) {

    utils::Linked_List <char*>* _line_preview = new utils::Linked_List <char*>();

    char* _preview_single_line;

    if (__line) {

        asprintf(
            &_preview_single_line,
            " %i | %s", 
            __line - 1,
            code_by_lines->operator[](
                __line - 1
            )
        );

        _line_preview->add(
            _preview_single_line
        );

    }

    asprintf(
        &_preview_single_line,
        " %i | %s", 
        __line,
        code_by_lines->operator[](
            __line
        )
    );

    _line_preview->add(
        _preview_single_line
    );

    if (__line < code_by_lines->count - 1) {

        asprintf(
            &_preview_single_line,
            " %i | %s", 
            __line + 1,
            code_by_lines->operator[](
                __line + 1
            )
        );

        _line_preview->add(
            _preview_single_line
        );

    }

    return _line_preview;

}

char* parser::Code_Information::get_line(int __line) {

    char* _preview_single_line;

    asprintf(
        &_preview_single_line,
        " %i | %s", 
        __line,
        code_by_lines->operator[](
            __line
        )
    );

    return _preview_single_line;

}

char* parser::Code_Information::get_line_padding(int __line) {

    int _number_length = snprintf( NULL, 0, "%d", __line);

    char* _padd = (char*) malloc(_number_length + 5);

    _padd[_number_length + 4] = '\0'; 
    for (int _ = 0; _ < _number_length + 4; _++) _padd[_] = ' '; 

    _padd[_number_length + 2] = '|'; 

    return _padd;

}

char* parser::Code_Information::get_line_error_sign(int __line, int __collumn_position, int __token_length) {

    char* _padding_line = 
        get_line_padding(__line);

    char* _error_sign = (char*) malloc(strlen(_padding_line) + __collumn_position + __token_length + 1);

    strncpy(
        _error_sign,
        _padding_line,
        strlen(_padding_line)
    );

    for (int _ = 0; _ < __collumn_position + __token_length - 1; _++)

        _error_sign[strlen(_padding_line) + _] = ' ';

    for (int _ = 0; _ < __token_length; _++)

        _error_sign[strlen(_padding_line) + __collumn_position + _] = '^';

    _error_sign[strlen(_padding_line) + __collumn_position + __token_length] = '\0';

    free(_padding_line);

    return _error_sign;

}

