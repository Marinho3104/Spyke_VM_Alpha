
#ifndef BUILT_INS_POINTER_H
#define BUILT_INS_POINTER_H

namespace built_ins {

    struct Void;
    struct Spyke;

    struct Pointer {

        Pointer(); Pointer(int); 

        void equal(int);

        spyke increment(int);

        spyke decrement(int);

        spyke increment_left(int);

        spyke decrement_left(int);

        spyke add(int);

        spyke sub(int);

        spyke add(int, int);

        spyke sub(int, int);

        spyke bracket(int, int);

    };

}

#endif