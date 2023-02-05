
namespace built_ins {

    struct Bool {};

    struct Int {

        bool add(bool) {}

        bool sub(int) {}

    };

    struct Pointer {

        int add(int) {}

        int add(int, int) {}

    };

}

namespace test {

    pointer lo;

    int* m, mm;

    int call_func(int*) {

    }

    bool test_func() {

        12 + (12 + (12 - 12));

    }

}
