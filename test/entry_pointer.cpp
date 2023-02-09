
#include "compiler.h"

#include <iostream>

struct Struct_Ola {

    int marinho;

};

void Struct_Ola() {

}

int main() {

    Struct_Ola::Struct_Ola __ola = Struct_Ola::Struct_Ola();

    // ola.Struct_Ola();

    compiler::Compiler* _compiler = new compiler::Compiler("./contract_test/contract_data.ph", COMPILER_MODE_FULL_COMPILATION);

    delete _compiler;

}





