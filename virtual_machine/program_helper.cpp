#include "program_helper.h"

#include "opcodes.h"

#include <iostream>

void virtual_machine::binary_arithmetrict_operations_0(void* __f, void* __s, short __sz, char __op) {

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

            *( (unsigned char*) __f ) = *( (unsigned char*) __s ) - *( (unsigned char*) __f ) - _over;        
            
            break;

        case BINARY_INCREMENT: 
                
                __ = ((short) *( (unsigned char*) __f ) + (!_ ? 1 : 0) > 255);

                *( (unsigned char*) __f ) = (!_ ? 1 : 0) + *( (unsigned char*) __f ) + _over;        
            
                break;

        case BINARY_DECREMENT: 

                __ = ((short) *( (unsigned char*) __f ) < 1);

                *( (unsigned char*) __f ) = *( (unsigned char*) __f ) - (!_ ? 1 : 0) - _over;        

                break;

        default: std::cout << "Error " << std::endl; exit(1); break;
        }
        
        _over = __;

        __f = __f + 1; __s = __s + 1;

    }

}

void virtual_machine::binary_comparation_operations(void* __f, void* __s, void* __r, short __sz, char __op) {

    switch (__op)
    {

        case BINARY_EQUAL_TO: case BINARY_NOT_EQUAL_TO:

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

                    default: break; // Never reatch

                }

                __f = __f + 1; __s = __s + 1;

            }

        case BINARY_GREATER_THAN: case BINARY_LESS_THAN: case BINARY_GREATER_THAN_EQUAL_TO: case BINARY_LESS_THAN_EQUAL_TO:

            __f = __f + __sz - 1; __s = __s + __sz - 1;

            for (short _ = __sz - 1; _ >= 0; _--) {

                switch (__op)
                {
                    case BINARY_GREATER_THAN: 

                        *( (bool*) __r) = 0;

                        if (
                            *( (unsigned char*) __s) < *( (unsigned char*) __f)
                        ) return;

                        else if (
                            *( (unsigned char*) __s) > *( (unsigned char*) __f)
                        ) {  *( (bool*) __r) = 1; return; } break;

                    case BINARY_LESS_THAN: 
                    
                        *( (bool*) __r) = 0;

                        if (
                            *( (unsigned char*) __s) > *( (unsigned char*) __f)
                        ) return;

                        else if (
                            *( (unsigned char*) __s) < *( (unsigned char*) __f)
                        ) {  *( (bool*) __r) = 1; return; } break;

                    case BINARY_GREATER_THAN_EQUAL_TO: 
                    
                        *( (bool*) __r) = 1;

                        if (
                            *( (unsigned char*) __s) < *( (unsigned char*) __f)
                        ) { *( (bool*) __r) = 0; return; }

                        else if (
                            *( (unsigned char*) __s) > *( (unsigned char*) __f)
                        ) return; break;

                    case BINARY_LESS_THAN_EQUAL_TO: 
                    
                        *( (bool*) __r) = 1;

                        if (
                            *( (unsigned char*) __s) > *( (unsigned char*) __f)
                        ) {  *( (bool*) __r) = 0; return; }

                        else if (
                            *( (unsigned char*) __s) < *( (unsigned char*) __f)
                        ) return; break;

                    default: break; // Never reatch
                }

                __f = __f - 1; __s = __s - 1;

            }

            break;

        default: std::cout << "Error " << std::endl; exit(1); break;

    }

}
