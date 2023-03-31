
#ifndef BUILT_INS_BOOL_H
#define BUILT_INS_BOOL_H

namespace built_ins {

    struct Void;
    struct Int;

    struct Bool {

        Bool(); Bool(bool); Bool(int);


        // Assing operations

        void equal(bool);

    };

}

#endif