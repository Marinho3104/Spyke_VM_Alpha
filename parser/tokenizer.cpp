#include "tokenizer.h"

#include "tokenizer_exceptions.h"
#include "token_definitions.h"
#include "tokenizer_helper.h"
#include "linked_list.h"
#include "common.h"
#include "token.h"

#include <iostream>


parser::Tokenizer::~Tokenizer() { delete tokens_collection; free(code_to_free); }

parser::Tokenizer::Tokenizer(Code_Information* __code_information, char* __code) 
    : code(__code), code_to_free(__code), collumn_inicial_address(__code), current_line(0), code_information(__code_information) 
        { tokens_collection = new utils::Linked_List <Token*>(); set_tokens(); }

void parser::Tokenizer::set_tokens() {

    while(*code) {

        if (*code <= 32) {

            if (*code == '\n') { current_line++; collumn_inicial_address = code + 1; }

            code++;

            continue;

        }

        handle_new_token();

    }

}

void parser::Tokenizer::print_tokens() {

    std::cout << "\n\t\t--> Tokenizer Tokens <---\n" << std::endl;

    for (int _ = 0; _ < tokens_collection->count; _++) { std::cout << "\t"; tokens_collection->operator[](_)->print(); }

}


void parser::Tokenizer::handle_new_token() {

    // Check if is symbol
    if (handle_token_symbol(0));

    // Check if is keyword
    else if (handle_token_keyword()); 

    // Check if is implicit value
    else if (handle_token_implicit_value());

    // Assume is identifier
    else handle_token_identifier();

}

bool parser::Tokenizer::handle_token_symbol(bool _check) {

    char* _previous_state = code;

    int _token_id;
    
    if (_check) {

        char* _temp_code = code;

        return parser::get_token_symbol_id( (const char*&)  _temp_code ) != -1;

    }
    
    else if ((_token_id = parser::get_token_symbol_id( (const char*&)  code )) == -1) return 0;     

    switch (_token_id)
    {
    case SINGLE_QUOTATION_MARK: handle_single_quotation_mark(); return 1; break;
    case COMMENT_SINGLE_LINE: handle_comment_single_line(); return 1; break;
    case QUOTATION_MARK: handle_quotation_mark(); return 1; break;
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

    tokens_collection->add(
        _token
    );

    return 1;

}

bool parser::Tokenizer::handle_token_keyword() {

    char* _previous_state = code;

    int _last_id = tokens_collection->last ? tokens_collection->last->object->id : -1;

    int _token_id = get_token_keyword_id( (const char*&)  code, _last_id );

    if (_token_id == -1) return 0;

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        _token_id, 
        0,
        (size_t) (_previous_state - collumn_inicial_address),
        current_line,
        (size_t) (code - _previous_state)
    );

    tokens_collection->add(
        _token
    );

    return 1;

}

bool parser::Tokenizer::handle_token_implicit_value() {

    char* _previous_state = code;

    while (*code > 32 && !handle_token_symbol(1)) code++;

    char* _identifier = 
        utils::get_string_copy_n(
            _previous_state,
            (size_t) (code - _previous_state)    
        );

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        IDENTIFIER, 
        _identifier,
        (size_t) (_previous_state - collumn_inicial_address),
        current_line,
        (size_t) (code - _previous_state)
    );

    tokens_collection->add(
        _token
    );

    if (is_int(_identifier, this)) _token->id = PRIMITIVE_TYPE_INT;

    else {

        code -= 
            tokens_collection->operator[](
                tokens_collection->count
            )->position_information.size    ;

        delete tokens_collection->remove(
            tokens_collection->count
        );

        return 0;

    }

    return 1;

}

void parser::Tokenizer::handle_token_identifier() {

    char* _previous_state = code;

    while (*code > 32 && !handle_token_symbol(1)) code++;

    char* _identifier = 
        utils::get_string_copy_n(
            _previous_state,
            (size_t) (code - _previous_state)    
        );

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        IDENTIFIER,
        _identifier,
        (size_t) (_previous_state - collumn_inicial_address),
        current_line,
        (size_t) (code - _previous_state)
    );

    tokens_collection->add(_token);

}


void parser::Tokenizer::handle_comment_single_line() { 
    
    while(1) {

        if (!*code || *code == '\n') break;
       
        ++code;

    }
    
}

void parser::Tokenizer::handle_comment() {

    int _token_comment_end_size = 2;

    bool _break = 0;

    int _previous_state_current_line = current_line;
    int _previous_state_current_position = code - collumn_inicial_address - _token_comment_end_size;

    while(1) {

        if (*code <= 32) {

            if (!*code) 
                throw Expected_Token_Exception_Tokenizer(code_information, COMMENT_END, _token_comment_end_size,  _previous_state_current_line, _previous_state_current_position); 
            //Ordinary_Exception_Tokenizer("Excpected token '*/' to finish comment");

            else if (*code == '\n') { current_line++; collumn_inicial_address = code + 1; }

            code++;

            continue;

        }

        handle_new_token();

        utils::Data_Linked_List <Token*>* _last_token_data_ll = 
            tokens_collection->remove(tokens_collection->count);

        _break = _last_token_data_ll->object->id == COMMENT_END;

        delete _last_token_data_ll;

        if (_break) break;

    }

}

void parser::Tokenizer::handle_quotation_mark() {

    char* _previous_state = code;

    int _token_quotation_mark_size = 1;

    int _previous_state_current_line = current_line;
    int _previous_state_current_position = code - collumn_inicial_address - _token_quotation_mark_size;

    while( *(code - 1) == '\\' || *code != '"' ) { 
        
        if (!*code)  
            throw Expected_Token_Exception_Tokenizer(code_information, QUOTATION_MARK, _token_quotation_mark_size,  _previous_state_current_line, _previous_state_current_position); 
        
        code++; 
    
    }

    char* _identifier = 
        utils::get_string_copy_n(
            _previous_state,
            (size_t) (code - _previous_state)    
        );

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        IMPLICIT_VALUE_STRING,
        _identifier,
        (size_t) (_previous_state - collumn_inicial_address - 1),
        current_line,
        (size_t) (code - _previous_state + 2)
    );

    tokens_collection->add(_token);

    code++;

}

void parser::Tokenizer::handle_single_quotation_mark() {

    char* _previous_state = code;

    int _token_single_quotation_mark_size = 1;

    int _previous_state_current_line = current_line;
    int _previous_state_current_position = code - collumn_inicial_address - _token_single_quotation_mark_size;

    while( *(code - 1) == '\\' || *code != '\'' ) { 
        
        if (!*code)  
            throw Expected_Token_Exception_Tokenizer(code_information, SINGLE_QUOTATION_MARK, _token_single_quotation_mark_size,  _previous_state_current_line, _previous_state_current_position); 
        
        code++; 
    
    }
    
    char* _identifier = 
        utils::get_string_copy_n(
            _previous_state,
            (size_t) (code - _previous_state)    
        );

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        IMPLICIT_VALUE_CHARACTER,
        _identifier,
        (size_t) (_previous_state - collumn_inicial_address - 1),
        current_line,
        (size_t) (code - _previous_state + 2)
    );

    tokens_collection->add(_token);

    code++;

}

