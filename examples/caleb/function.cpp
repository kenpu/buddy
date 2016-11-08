/**
 * Describe a function where:
 *
 * Domain {0, 1, 2, 3, 4, 5}
 * Range {0, 1, 2}
 *
 * where forall(i, j) 
 *     f(i) == 1 & f(j) == 2 -> i <= j
 */

#include "fdd.h"
#include "bvec.h"

using namespace std;

const int D = 6; //Domain Size
const int R = 3; //Range Size

int main() {

    bdd_init(1000000, 1000000);

    int domains[D];

    fill_n(domains, D, R);
    
    fdd_extdomain(domains, D);

    bvec *outputs = new bvec[D];

    for (int i = 0; i < D; i++) {
        outputs[i] = bvec_varfdd(i);
    }

    //All outputs must be 2 or less
    bdd c1 = bddtrue;
    bvec two = bvec_con(outputs[0].bitnum(), 2);
    for (int i = 0; i < D; i++) {
        c1 &= bvec_lte(outputs[i], two);
    }

    //Every output must be less than or equal to the next
    bdd c2 = bddtrue;
    for (int i = 0; i < D - 1; i++) {
        for (int j = i + 1; j < D; j++) {
            c2 &= bvec_lte(outputs[i], outputs[j]);
        }
    }

    bdd c = c1 & c2;

    cout << fddset << c << endl;

    return 0;
}
