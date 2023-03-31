

#include "./contract_test/pointer.h"
#include "./contract_test/spyke.h"



void built_ins::Pointer::Pointer() {}

void built_ins::Pointer::Pointer(int) { exec 4, 4; exec 4, 6; exec 12, 0; exec 9, 2; }

void built_ins::Pointer::Pointer(pointer) { exec 4, 2; exec 4, 4; exec 12, 0; exec 9, 2; }


// Assign operations

void built_ins::Pointer::equal(int) { exec 4, 4; exec 4, 6; exec 12, 0; exec 9, 2; }

spyke built_ins::Pointer::increment(int __s) { *this = *this + __s; return *this - __s; }

spyke built_ins::Pointer::decrement(int __s) { *this = *this - __s; return *this + __s; }

spyke built_ins::Pointer::increment_left(int __s) { *this = *this + __s; return *this; }

spyke built_ins::Pointer::decrement_left(int __s) { *this = *this - __s; return *this; }


// 

spyke built_ins::Pointer::add(int __off) { exec 4, 4; exec 4, 6; exec 12, 0; exec 16, 2; return __off; }

spyke built_ins::Pointer::sub(int __off) { pointer _ = *this; exec 4, 2; exec 4, 6; exec 17, 2; return _; }

spyke built_ins::Pointer::add(int __c, int __s) 
    { __s = __c * __s; return *this + __s; }

spyke built_ins::Pointer::sub(int __c, int __s) 
    { __s = __c * __s; return *this - __s; }

spyke built_ins::Pointer::bracket(int __c, int __s) 
    { __s = __c * __s; pointer _ = *this + __s; exec 4, 2; exec 12, 0; }



