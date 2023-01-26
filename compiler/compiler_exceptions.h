#pragma once

#ifndef COMPILER_EXCEPTIONS_H
#define COMPILER_EXCEPTIONS_H

#include <exception>
#include <iostream>

namespace compiler {

    struct Ordinary_Exception : public std::exception {

        const char* information;

        Ordinary_Exception(const char* __info);

        const char* what() const throw();

    };


}

#endif