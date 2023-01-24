#include "common.h"

#include <iostream>
#include <string.h>
#include <fstream>

char* utils::get_string_copy(char* __to_copy) {

    char* __new = (char*) malloc(strlen(__to_copy) + 1);

    strcpy(
        __new,
        __to_copy
    );

    return __new;

}

char* utils::get_file_content(char* __path) {

    std::ifstream ifs(__path);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
        (std::istreambuf_iterator<char>()    ) );

    char* _file_content = (char*) malloc(strlen(content.c_str()) + 1);

    strcpy(_file_content, (char*) content.c_str());

    return _file_content;

} 
