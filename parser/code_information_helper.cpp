#include "code_information_helper.h"

#include <iostream>


size_t parser::get_character_position(char* __str, char __chr) {

    size_t _count = 0;

    while(*__str) {

        if (*(__str++) == __chr) break;
        
        ++_count;

    }

    return _count;

}
