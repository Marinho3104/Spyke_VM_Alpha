
#include "linked_list.h"
#include "byte_code.h"
#include "convertor.h"
#include "compiler.h"
#include "program.h"

#include <iostream>

int main() {

    compiler::Compiler* _compiler = new compiler::Compiler("./contract_test/contract_data.ph", COMPILER_MODE_FULL_COMPILATION);

    int _initial_block = _compiler->convertor->entry_points->first->object;
    int _entry_point = _compiler->convertor->entry_points->last->object;

    delete _compiler;

    byte_code::Byte_Code_File* _byte_code_file = byte_code::Byte_Code_File::get_file_content("file.byte_code");

    virtual_machine::Program* _program = new virtual_machine::Program(_initial_block, _entry_point, _byte_code_file->content, _byte_code_file->size);

    _program->execute_program();

    delete _byte_code_file;

    delete _program;

}





