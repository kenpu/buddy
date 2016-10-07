#include <stdio.h>
#include "bdd.h"
#include "fdd.h"
#include "bvec.h"

using namespace std;

bdd alldistinct(bvec *list, int n) {
    bdd c = bddtrue;
    for(int i=0; i < n; i++) {
        bvec x = list[i];
        for(int j=i+1; j < n; j++) {
            bvec y = list[j];
            c &= (x != y);
        }
    }
    return c;
}

// |x-y| <= 1
bdd neighbour(bvec x, bvec y) {
    int b = x.bitnum();
    bvec val2 = bvec_con(b, 2);
    bdd c1 = bdd_imp((x < y), ((y - x) < val2));
    bdd c2 = bdd_imp((x >= y), ((x - y) < val2));

    return c1 & c2;
}

int main() {
    bdd_init(100000, 100000);
    int N = 3;
    int domains[3] = {400, 400, 400};
    fdd_extdomain(domains, 3);

    bvec P1 = bvec_varfdd(0);
    bvec P2 = bvec_varfdd(1);
    bvec P3 = bvec_varfdd(2);
    // bvec P4 = bvec_varfdd(3);
    // bvec P5 = bvec_varfdd(4);
    
    printf("test bvec val: %d\n", bvec_val(P3));

    // bvec everyone[N] = {P1, P2, P3, P4, P5};
    bvec everyone[3] = {P1, P2, P3};

    bdd C1 = alldistinct(everyone, N);
    bdd C2 = neighbour(P1, P2);
    bdd C3 = neighbour(P1, P3);

    bdd constraint = C1 & !C2 & !C3;

    printf("All done. # of solutions: %ld\n", (long)bdd_satcount(constraint));
}
