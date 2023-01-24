#include "compiler.h"

#include "compiler_exceptions.h"
#include "common.h"

/* Parser */
#include "tokenizer.h"

#include <iostream>


compiler::Compiler::~Compiler() { 
    
    if (tokenizer) delete tokenizer;
    
    free(code);  
    
}

compiler::Compiler::Compiler(const char* __path, int __compilation_mode) {

    code = utils::get_file_content( (char*) __path );

    switch (__compilation_mode)
    {
    case COMPILER_MODE_FULL_COMPILATION: full_compilation(); break;
    default: throw Ordinary_Exception("Unknow Compiler Mode"); break;
    }

}

void compiler::Compiler::full_compilation() {

    tokenizer = 
        new parser::Tokenizer(
            utils::get_string_copy(
                code
            )
        );

}









