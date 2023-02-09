
namespace built_ins {

    struct Void {};

    struct Bool {};

    struct Int {

        int add(int) {}

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

    struct Struct_Test {

        ~Struct_Test();
        
        Struct_Test();

        Struct_Test(int);

        Struct_Test* l;

        Struct_Test ola();

    };

    bool test_func() {

        Struct_Test _ola;

        _ola.~Struct_Test(12);



    }

}
