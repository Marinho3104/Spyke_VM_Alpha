#pragma once

#ifndef CONVERTOR_EXCEPTIONS_H
#define CONVERTOR_EXCEPTIONS_H

#include "parser_definitions.h"

#include <exception>
#include <iostream>

namespace parser {

    struct Ordinary_Exception_Convertor : public std::exception {

        const char* information;

        Ordinary_Exception_Convertor(const char* __info);

        const char* what() const throw();

    };


}

#endif