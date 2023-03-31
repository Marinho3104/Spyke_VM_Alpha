
#include "./contract_test/int.h"
#include "./contract_test/bool.h"


// Constructors

void built_ins::Int::Int() {}

void built_ins::Int::Int(int __eq) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 9, 4; }


// Arithmetric operations

int built_ins::Int::add(int __add) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 16, 4; return __add; }

int built_ins::Int::sub(int __sub) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 17, 4; return __sub; }

int built_ins::Int::mul(int __mul) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 18, 4; return __mul; }

int built_ins::Int::div(int __div) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 19, 4; return __div; }

int built_ins::Int::mod(int __mod) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 20, 4; return __mod; }



// Assign operations

void built_ins::Int::equal(int) 
    { exec 4, 4; exec 4, 6; exec 12, 0; exec 9, 4; } 

void built_ins::Int::add_assign(int) 
    { exec 4, 6; exec 12, 0; exec 4, 4; exec 16, 4; }



// Others 

int built_ins::Int::increment() 
    { exec 4, 2; exec 12, 0; exec 30, 4; return *this - 1; }

int built_ins::Int::decrement() 
    { exec 4, 2; exec 12, 0; exec 31, 4; return *this + 1; }

int built_ins::Int::increment_left() 
    { exec 4, 2; exec 12, 0; exec 30, 4; return *this; }

int built_ins::Int::decrement_left() 
    { exec 4, 2; exec 12, 0; exec 31, 4; return *this; }



// Comparation

bool built_ins::Int::equal_to(int) 
    { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 24, 4; return _rtr; }

bool built_ins::Int::not_equal_to(int) 
    { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 25, 4; return _rtr; }

bool built_ins::Int::greater_than(int) 
    { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 26, 4; return _rtr; }

bool built_ins::Int::less_than(int) 
    { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 27, 4; return _rtr; }

bool built_ins::Int::greater_than_equal_to(int) 
    { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 28, 4; return _rtr; }

bool built_ins::Int::less_than_equal_to(int) 
    { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 29, 4; return _rtr; }
