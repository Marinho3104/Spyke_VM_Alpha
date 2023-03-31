#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

#include "byte_code_definitions.h"

#define CODE_SIZE 1
#define ARGUMENT_SIZE 2

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

        utils::Linked_List <int>* custom_sizes, *custom_sizes_h;

        int current_stack_size;

        bool entry_point;

        ~Byte_Code_Block(); Byte_Code_Block(utils::Linked_List <Byte_Code*>*); Byte_Code_Block();


        byte_code::Byte_Code* get_load_byte_code(short);

        static byte_code::Byte_Code* get_load_global_byte_code(short, byte_code::Byte_Code_Block*);


    };

    struct Compiled_Byte_Code {

        utils::Linked_List <Byte_Code_Block*>* blocks;
        int implicit_values_size;
        void* implicit_values;

        ~Compiled_Byte_Code(); Compiled_Byte_Code(utils::Linked_List <Byte_Code_Block*>*);

        void set_implicit_values(utils::Linked_List <parser::Token*>*, int);


        void save_in_file(const char*);

        void print();

    };

    struct Byte_Code_File {

        void* content;
        short size;

        ~Byte_Code_File(); Byte_Code_File(void*, short);

        static Byte_Code_File* get_file_content(const char*);


    };

}

#endif