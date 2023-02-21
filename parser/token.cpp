#include "token.h"

#include "token_definitions.h"
#include "common.h"

#include <iostream>
#include "string.h"

parser::Position_Information::Position_Information(size_t __column, size_t __line, size_t __size) : column(__column), line(__line), size(__size) {}


parser::Token::~Token() { free(identifier); }

parser::Token::Token(int __id, char* __identifier) 
    : id(__id), identifier(__identifier), position_information(Position_Information(0, 0, 0)) {}

parser::Token::Token(int __id, char* __identifier, size_t __collumn, size_t __line, size_t __size) 
    : id(__id), identifier(__identifier), position_information(Position_Information(__collumn, __line, __size)) {}

parser::Token::Token(int __id, char* __identifier, Position_Information __position_information)
    : id(__id), identifier(__identifier), position_information(__position_information) {}

bool parser::Token::operator==(Token* __token) {

    return (
        id == __token->id &&
        !strcmp(identifier, __token->identifier)
    );

}

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


bool parser::is_primitive_type(int __token_id) { return __token_id >= PRIMITIVE_TYPE_VOID && __token_id <= PRIMITIVE_TYPE_POINTER; }

bool parser::is_implicit_value_type(int __token_id) { return __token_id >= IMPLICIT_VALUE_INT && __token_id <= IMPLICIT_VALUE_STRING; }

bool parser::is_single_parameters_function_operation(int __token_id) { 
    
    return (
        (__token_id >= FUNCTION_OPERATOR_INCREMENT && __token_id <= FUNCTION_OPERATOR_DECREMENT_LEFT) || 
        __token_id == FUNCTION_OPERATOR_NOT || 
        __token_id == FUNCTION_OPERATOR_BITWISE_NOT
    ); 
    
}

int parser::get_primitive_type_of_implicit_value_type(int __implicit_value_type) {

    switch (__implicit_value_type)
    {
    case IMPLICIT_VALUE_INT: return PRIMITIVE_TYPE_INT; break;
    default: break;
    }

    std::cout << "Error implicti conversio to primitve TODO" << std::endl;

    exit(1);

    return -1;

}
