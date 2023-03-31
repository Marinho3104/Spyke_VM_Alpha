
#ifndef BUILT_INS_INT_H
#define BUILT_INS_INT_H

namespace built_ins {

    struct Bool;
    struct Void;


    struct Int {

        Int(); Int(int __eq);


        // Arithmetrict operations

        int add(int); int sub(int); int mul(int); int div(int); int mod(int);


        // Assing operations

        void equal(int); void add_assign(int);

        // Others

        int increment(); int decrement(); int increment_left(); int decrement_left();

        // Comparation

        bool equal_to(int); bool not_equal_to(int); bool greater_than(int); bool less_than(int); 
        
        bool greater_than_equal_to(int); bool less_than_equal_to(int);

    };

}

#endif
