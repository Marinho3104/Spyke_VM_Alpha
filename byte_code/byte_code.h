#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

#include "byte_code_definitions.h"

namespace byte_code {

    struct Byte_Code {

        char code;
        short argument;

        ~Byte_Code(); Byte_Code(char, short);

    };

    struct Byte_Code_Block {

        utils::Linked_List <Byte_Code*>* block;

        ~Byte_Code_Block(); Byte_Code_Block(utils::Linked_List <Byte_Code*>*); Byte_Code_Block();

    };

}

#endif