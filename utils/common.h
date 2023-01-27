#pragma once

#ifndef UTILS_COMMON_H
#define UTILS_COMMON_H

#include <stddef.h>

namespace utils {

    char* get_string_copy(char*);

    char* get_string_copy_n(char*, size_t);

    bool file_exists(char*);

    char* get_file_content(char*);

    

}

#endif