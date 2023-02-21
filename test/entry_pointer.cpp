
#include "compiler.h"

#include <iostream>

struct Test_Struct {

    bool operator&&(Test_Struct) {}

    Test_Struct() {}

    void operator=(Test_Struct) {

        std::cout << "Hey" << std::endl;

    }

    Test_Struct operator+(int) {
        
        std::cout << "Hey" << std::endl;

    }

};

int main() {

    for (int _ = 0; _ < 10 ; _++ ) {
        
    }

    compiler::Compiler* _compiler = new compiler::Compiler("./contract_test/contract_data.ph", COMPILER_MODE_FULL_COMPILATION);

    delete _compiler;

}





