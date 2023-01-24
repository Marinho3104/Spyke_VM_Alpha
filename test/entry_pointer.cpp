
#include "compiler.h"

#include <iostream>

int main() {

    compiler::Compiler* _compiler = new compiler::Compiler("./contract_test/contract_data.ph", COMPILER_MODE_FULL_COMPILATION);

    delete _compiler;

}