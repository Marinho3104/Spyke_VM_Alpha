
#include "./contract_test/bool.h"


void built_ins::Bool::Bool() {}

void built_ins::Bool::Bool(bool) 
    { exec 4, 1; exec 4, 3; exec 12, 0; exec 9, 1; }

void built_ins::Bool::Bool(int __v) 
    { *this = __v != 0; }



// Assign operations

void built_ins::Bool::equal(bool) 
    { exec 4, 1; exec 4, 3; exec 12, 0; exec 9, 1; }

