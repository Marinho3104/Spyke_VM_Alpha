#include "byte_code.h"

#include "byte_code_helper.h"
#include "linked_list.h"
#include "ast_helper.h"

#include <iostream>


byte_code::Byte_Code::~Byte_Code() {}

byte_code::Byte_Code::Byte_Code(char __code, short __argument) : code(__code), argument(__argument) {}

void byte_code::Byte_Code::print() { std::cout << "Byte Code -> " << (int) code << " argument -> " << (int) argument << std::endl; }

byte_code::Byte_Code* byte_code::Byte_Code::generate(char __code, short __argument) {

    Byte_Code* _byte_code = (Byte_Code*) malloc(sizeof(Byte_Code));

    new (_byte_code) Byte_Code(
        __code, __argument
    );

    return _byte_code;

}


byte_code::Byte_Code_Block::~Byte_Code_Block() { delete block; }

byte_code::Byte_Code_Block::Byte_Code_Block(utils::Linked_List <Byte_Code*>* __block) : block(__block), current_stack_size(0), entry_point(0) {}

byte_code::Byte_Code_Block::Byte_Code_Block() : current_stack_size(0), entry_point(0) { block = new utils::Linked_List <Byte_Code*>(); }


byte_code::Compiled_Byte_Code::~Compiled_Byte_Code() { free(implicit_values); }

byte_code::Compiled_Byte_Code::Compiled_Byte_Code(utils::Linked_List <Byte_Code_Block*>* __blocks) : blocks(__blocks), implicit_values(0), implicit_values_size(0) {}

void byte_code::Compiled_Byte_Code::set_implicit_values(utils::Linked_List <parser::Token*>* __implicit_values, int __size) {

    implicit_values = malloc(__size);
    implicit_values_size = __size;

    if (!implicit_values_size) return;

    void* _current_position = implicit_values;

    for (int _ = 0; _ < __implicit_values->count; _++) {

        set_implicit_value_content(_current_position, __implicit_values->operator[](_));

        _current_position = _current_position + get_implicit_value_type_size(__implicit_values->operator[](_));

    }

}

void byte_code::Compiled_Byte_Code::save_in_file(const char* __file_name) {

    FILE* _file = 
        fopen(__file_name, "wb");

    if (implicit_values_size)

        fwrite(
            implicit_values,
            implicit_values_size, 1,
            _file
        );

    for (int _ = 0; _ < blocks->count; _++)

        for (int __ = 0; __ < blocks->operator[](_)->block->count; __++) {

            fwrite(
                &blocks->operator[](_)->block->operator[](__)->code,
                CODE_SIZE, 1,
                _file
            );

            fwrite(
                &blocks->operator[](_)->block->operator[](__)->argument,
                ARGUMENT_SIZE, 1,
                _file
            );

        }

    fclose(_file);

}

void byte_code::Compiled_Byte_Code::print() {

    for (int _ = 0; _ < blocks->count; _++) {

        std::cout << "\tBlock: " << _ << "\n" << std::endl;

        for (int __ = 0; __ < blocks->operator[](_)->block->count; __++) {

            std::cout << "\t\t";

            blocks->operator[](_)->block->operator[](__)->print();

        }

        std::cout << std::endl;


    }    

}



byte_code::Byte_Code_File::~Byte_Code_File() { free(content); }

byte_code::Byte_Code_File::Byte_Code_File(void* __content, short __size) 
    : content(__content), size(__size) {}

byte_code::Byte_Code_File* byte_code::Byte_Code_File::get_file_content(const char* __file_path) {

    FILE* _file = fopen(__file_path, "rb");

    fseek(_file, 0, SEEK_END);

    short _lenght = ftell(_file);

    fseek(_file, 0, SEEK_SET);

    void* _content = malloc(_lenght);

    fread(
        _content,
        _lenght,
        1,
        _file
    );

    fclose(_file);

    return 
        new Byte_Code_File(
            _content, _lenght
        );

}