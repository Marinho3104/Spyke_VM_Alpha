#include "token.h"

#include "common.h"

#include <iostream>


parser::Position_Information::Position_Information(size_t __column, size_t __line, size_t __size) : column(__column), line(__line), size(__size) {}


parser::Token::~Token() { free(identifier); }

parser::Token::Token(int __id, char* __identifier, size_t __collumn, size_t __line, size_t __size) 
    : id(__id), identifier(__identifier), position_information(Position_Information(__collumn, __line, __size)) {}

parser::Token::Token(int __id, char* __identifier, Position_Information __position_information)
    : id(__id), identifier(__identifier), position_information(__position_information) {}

void parser::Token::print() {

    std::cout << "Token Information: id -> " << id << " | identifier -> " << (identifier ? identifier : "--- None ---") <<
        " | Position Information -> " << position_information.line << ":" << position_information.column  << " | Size -> " << position_information.size << std::endl;

}

parser::Token* parser::Token::get_copy() {

    parser::Token* _token = (parser::Token*) malloc(sizeof(parser::Token));

    new (_token) parser::Token(
        id,
        utils::get_string_copy(
            identifier
        ),
        position_information
    );

    return _token;

}
