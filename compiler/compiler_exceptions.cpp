#include "compiler_exceptions.h"

#include <iostream>

compiler::Ordinary_Exception::Ordinary_Exception(const char* __info) : information(__info) {}

const char* compiler::Ordinary_Exception::what() const throw() { return information; }
