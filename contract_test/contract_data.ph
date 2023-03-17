
namespace built_ins {

    struct Void;

    struct Int;

    struct Bool {

        ~Bool() {

        }

        Bool() {

        }

        Bool(bool) { exec 4, 1; exec 4, 3; exec 12, 0; exec 9, 1; }

    };

    struct Int {

        ~Int() {}

        Int() {}

        int add(int __add) { exec 4, 4; exec 4, 6; exec 12, 0; exec 16, 4; return __add; }

        int sub(int __sub) { exec 4, 4; exec 4, 6; exec 12, 0; exec 17, 4; return __sub; }

        int mul(int __mul) { exec 4, 4; exec 4, 6; exec 12, 0; exec 18, 4; return __mul; }

        int div(int __div) { exec 4, 4; exec 4, 6; exec 12, 0; exec 19, 4; return __div; }

        int mod(int __mod) { exec 4, 4; exec 4, 6; exec 12, 0; exec 20, 4; return __mod; }

        void add_assign(int __add) { exec 4, 6; exec 12, 0; exec 4, 4; exec 16, 4; }

        void equal(int) { exec 4, 4; exec 4, 6; exec 12, 0; exec 9, 4; } 

        bool equal_to(int) { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 24, 4; return _rtr; }

        bool not_equal_to(int) { bool _rtr; exec 4, 1; exec 4, 5; exec 4, 7; exec 12, 0; exec 25, 4; return _rtr; }

        Int(int __eq) { *this = __eq; }

    };

    struct Pointer {

        Pointer() {

        }

        void equal(int) { exec 4, 4; exec 4, 6; exec 12, 0; exec 9, 2; }

        void add_assign(int) { exec 4, 6; exec 12, 0; exec 4, 4; exec 16, 2; }

        void add_assign(int __c, int __s) { __c = __c * __s; exec 4, 8; exec 4, 10; exec 12, 0; exec 4, 8; exec 16, 2; }

    };

}

namespace test {

    void func() {

        int mario = 100;

        bool val = mario != 150;

        exec 4, 1;

    }

}


