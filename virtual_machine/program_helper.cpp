#include "program_helper.h"

#include "opcodes.h"

#include <iostream>

void virtual_machine::binary_arithmetrict_operations(void* __f, void* __s, short __sz, char __op) {

    bool _over = 0, __;

    for (short _ = 0; _ < __sz; _++) {

        // std::cout << "First -> " << (int) *( (unsigned char*) __f ) << std::endl; 
        // std::cout << "Second -> " << (int) *( (unsigned char*) __s ) << std::endl; 

        switch (__op)
        {
        case BINARY_ADD: 
        
            __ = ((short) *( (unsigned char*) __f ) + *( (unsigned char*) __s ) > 255);

            *( (unsigned char*) __f ) = *( (unsigned char*) __s ) + *( (unsigned char*) __f ) + _over;        
        
            break;

        case BINARY_SUB: 
        
            __ = *( (unsigned char*) __f ) > *( (unsigned char*) __s );

            *( (unsigned char*) __f ) = *( (unsigned char*) __s ) - *( (unsigned char*) __f ) + _over;        
            
            break;

        case BINARY_MUL: 
        
            __ = ((short) *( (unsigned char*) __f ) * *( (unsigned char*) __s ) > 255);

            *( (unsigned char*) __f ) = *( (unsigned char*) __s ) * (*( (unsigned char*) __f ) + _over);          

            break;

        case BINARY_DIV: 

            __ = *( (unsigned char*) __f ) > *( (unsigned char*) __s );

            if ( ! *( (unsigned char*) __f ) ) break;

            *( (unsigned char*) __f ) = *( (unsigned char*) __s ) / (*( (unsigned char*) __f ) + _over);
        
            break;


        case BINARY_MOD: 

            __ = *( (unsigned char*) __f ) > *( (unsigned char*) __s );

            if ( ! *( (unsigned char*) __f ) ) break;

            *( (unsigned char*) __f ) = *( (unsigned char*) __s ) % *( (unsigned char*) __f + _over);
        
            break;

        default: std::cout << "Error " << std::endl; exit(1); break;
        }
        
        _over = __;

        __f = __f + 1; __s = __s + 1;

    }

}

void virtual_machine::binary_comparation_operations(void* __f, void* __s, void* __r, short __sz, char __op) {

    for (short _ = 0; _ < __sz; _++) {

        switch (__op)
        {

            case BINARY_EQUAL_TO:

                *( (bool*) __r) = 1;

                if (
                    *( (unsigned char*) __s ) != *( (unsigned char*) __f)
                ) { *( (bool*) __r) = 0; return; } break;

            case BINARY_NOT_EQUAL_TO:

                *( (bool*) __r) = 0;

                if (
                    *( (unsigned char*) __s ) != *( (unsigned char*) __f)
                ) { *( (bool*) __r) = 1; return; } break;

            default: std::cout << "Error " << std::endl; exit(1); break;

        }

        __f = __f + 1; __s = __s + 1;

    }

}
