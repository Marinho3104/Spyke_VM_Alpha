#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

#include "byte_code_definitions.h"

namespace byte_code {

    struct Byte_Code {

        char code;
        short argument;

        ~Byte_Code(); Byte_Code(char, short);

        void print();

        static Byte_Code* generate(char, short);

    };

    struct Byte_Code_Block {

        utils::Linked_List <Byte_Code*>* block;

        int current_stack_size;

        ~Byte_Code_Block(); Byte_Code_Block(utils::Linked_List <Byte_Code*>*); Byte_Code_Block();

    };

    struct Compiled_Byte_Code {

        utils::Linked_List <Byte_Code_Block*>* blocks;

        ~Compiled_Byte_Code(); Compiled_Byte_Code(utils::Linked_List <Byte_Code_Block*>*);

        void print();

    };

}

#endif