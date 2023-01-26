#include "pre_compiler_warnings.h"

#include "code_information.h"
#include "linked_list.h"

#include <iostream>
#include <string.h>

void parser::More_Arguments_Than_Needed_Warning::launch(Code_Information* __code_information, int __instruction_id, int __line, int __column) {

    char* _code_line = 
        __code_information->get_line(__line);

    char* _error_sign = 
        __code_information->get_line_error_sign(
            __line, 
            __column, 
            strlen(
                __code_information->code_by_lines->operator[](
                    __line
                )
            ) - __column
        );

    std::cout << "\n\n\tWarning:";

    std::cout << "\n\n\t\tMore Arguments Than Needed for given Pre Compiler Instruction (id:" << __instruction_id << "):";

    std::cout << "\n\n\t\t\t" << _code_line;

    std::cout << "\n\t\t\t" << _error_sign << std::endl;
    
    free(_code_line);
    free(_error_sign);

}
