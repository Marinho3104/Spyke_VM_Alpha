
#include "linked_list.h"
#include "byte_code.h"
#include "convertor.h"
#include "compiler.h"
#include "program.h"

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

    compiler::Compiler* _compiler = new compiler::Compiler("./contract_test/contract_data.ph", COMPILER_MODE_FULL_COMPILATION);

    int _entry_point = _compiler->convertor->entry_points->last->object;

    delete _compiler;

    byte_code::Byte_Code_File* _byte_code_file = byte_code::Byte_Code_File::get_file_content("file.byte_code");

    virtual_machine::Program* _program = new virtual_machine::Program(_entry_point, _byte_code_file->content, _byte_code_file->size);

    _program->execute();

    delete _byte_code_file;

    delete _program;

}





