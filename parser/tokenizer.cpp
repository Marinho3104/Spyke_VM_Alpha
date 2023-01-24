#include "tokenizer.h"

#include "token_definitions.h"
#include "tokenizer_helper.h"
#include "linked_list.h"
#include "token.h"

#include <iostream>


parser::Tokenizer::~Tokenizer() { delete tokens_collection; free(code_to_free); }

parser::Tokenizer::Tokenizer(char* __code) : code(__code), code_to_free(__code), collumn_inicial_address(__code), current_line(0) 
    { tokens_collection = new utils::Linked_List <Token*>(); set_tokens(); }

void parser::Tokenizer::set_tokens() {

    while(*code) {

        if (*code <= 32) {

            if (*code == '\n') { current_line++; collumn_inicial_address = code + 1; }

            code++;

        }

        handle_new_token();

    }

    print_tokens();

}

void parser::Tokenizer::print_tokens() {

    std::cout << "\n\t\t--> Tokenizer Tokens <---\n" << std::endl;

    for (int _ = 0; _ < tokens_collection->count; _++) { std::cout << "\t"; tokens_collection->operator[](_)->print(); }

}


void parser::Tokenizer::handle_new_token() {

    // Check if is symbol
    if (handle_token_symbol());

    // Check if is keyword
    else if (0); 

    // Assume is identifier
    else;

}

bool parser::Tokenizer::handle_token_symbol() {

    char* _previous_state = code;

    int _token_id = parser::get_token_symbol_id( (const char*&)  code);

    if (_token_id == -1) return 0;

    switch (_token_id)
    {
    case COMMENT_SINGLE_LINE: handle_comment_single_line(); return 1; break;
    case COMMENT: handle_comment(); return 1; break;
    default: break;
    }

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        _token_id,
        0,
        (size_t) (_previous_state - collumn_inicial_address),
        current_line,
        (size_t) (code - _previous_state)
    );

    switch (_token_id)
    {
    case QUOTATION_MARK: handle_quotation_mark(); break;
    case SINGLE_QUOTATION_MARK: handle_single_quotation_mark(); break;
    default: break;
    }

    tokens_collection->add(
        _token
    );

    return 1;

}


void parser::Tokenizer::handle_comment_single_line() {

}

void parser::Tokenizer::handle_comment() {

}

void parser::Tokenizer::handle_quotation_mark() {

}

void parser::Tokenizer::handle_single_quotation_mark() {

}
