#include <stdio.h>
#include "bdd.h"
#include "fdd.h"
#include "bvec.h"

using namespace std;

bvec mul(bvec a, bvec b) {
    int n = a.bitnum();
    return bvec_mul(a, bvec_coerce(n, b));
}
bvec add(bvec a, bvec b) {
    int n = a.bitnum();
    return bvec_add(a, bvec_coerce(n, b));
}
bdd equ(bvec a, bvec b) {
    int n = a.bitnum();
    return bvec_equ(a, bvec_coerce(n, b));
}

int main(int argc, char **argv) {
    bdd_init(100000,100000);

    int domains[3] = {120, 120, 120};
    fdd_extdomain(domains, 3);

    bvec x = bvec_varfdd(0);
    bvec y = bvec_varfdd(1);
    bvec z = bvec_varfdd(2);

    bvec zero = bvec_con(x.bitnum(), 0);

    bvec xx = mul(x, x);
    bvec yyy = mul(mul(y, y), y);
    bvec zz = mul(z, z);
    bdd c = equ(add(yyy, xx), zz) & bvec_gth(x, zero)
                                  & bvec_gth(y, zero)
                                  & bvec_gth(z, zero);

    cout << fddset << c << endl;

    return 0;
}
