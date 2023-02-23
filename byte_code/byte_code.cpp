#include "byte_code.h"

#include "linked_list.h"

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

byte_code::Byte_Code_Block::Byte_Code_Block(utils::Linked_List <Byte_Code*>* __block) : block(__block), current_stack_size(0) {}

byte_code::Byte_Code_Block::Byte_Code_Block() : current_stack_size(0) { block = new utils::Linked_List <Byte_Code*>(); }


byte_code::Compiled_Byte_Code::~Compiled_Byte_Code() {}

byte_code::Compiled_Byte_Code::Compiled_Byte_Code(utils::Linked_List <Byte_Code_Block*>* __blocks) : blocks(__blocks) {}

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


