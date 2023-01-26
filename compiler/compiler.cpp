#include "compiler.h"

#include "compiler_exceptions.h"
#include "common.h"

/* Parser */
#include "code_information.h"
#include "pre_compiler.h"
#include "tokenizer.h"

#include <iostream>


compiler::Compiler::~Compiler() { 
    
    if (code_information) delete code_information;
    if (tokenizer) delete tokenizer;
    if (pre_compiler) delete pre_compiler;
    
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

    code_information = 
        new parser::Code_Information(
            utils::get_string_copy(
                code
            )
        );

    tokenizer = 
        new parser::Tokenizer(
            code_information,
            utils::get_string_copy(
                code
            )
        );

    pre_compiler = 
        new parser::Pre_Compiler(
            code_information,
            tokenizer->tokens_collection
        );

}









