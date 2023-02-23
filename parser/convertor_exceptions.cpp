#include "convertor_exceptions.h"

#include <iostream>


parser::Ordinary_Exception_Convertor::Ordinary_Exception_Convertor(const char* __info) : information(__info) {}

const char* parser::Ordinary_Exception_Convertor::what() const throw() { return information; }