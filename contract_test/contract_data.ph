
#include "./contract_test/void.h"
#include "./contract_test/int.h"
#include "./contract_test/bool.h"
#include "./contract_test/pointer.h"
#include "./contract_test/spyke.h"

#include "./contract_test/int.ph"
#include "./contract_test/bool.ph"
#include "./contract_test/pointer.ph"
#include "./contract_test/spyke.ph"


namespace test {

    spyke ff() { return 100; }

    void jj() {}

    void func() {

        int m = 0;

        for ( ; m < 100; m++) {  }

        exec 4, 4;

    }

}


/*

        int m;

        int mk[5];

        pointer _scnd = (mk + 1);

        // *_scnd = 1423;

        int mm = mk[1];

        exec 4, 4;

*/
