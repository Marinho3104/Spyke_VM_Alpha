
namespace built_ins {

    struct Void {};

    struct Bool {};

    struct Int {

        Int(int);

        int add(bool) {}

        bool sub(int) {}

    };

    struct Pointer {

        int add(int) {}

        int add(int, int) {}

        int testing_function_hello(int);

    };

}

namespace popo {

int call_func(int*) {

}

}

namespace test_1 {

    bool test_func() {

        1 + ::built_ins::Int(); // Error dont work out side built_ins namespace TODO

    }

}
