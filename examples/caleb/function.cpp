

#include "fdd.h"
#include "bvec.h"

using namespace std;

int main() {

    bdd_init(1000000, 1000000);

    int D = 6;
    int R = 3;

    int domain[6] = {3, 3, 3, 3, 3, 3};

    fdd_extdomain(domain, D);

    bvec *items = new bvec[D];
    for (int i = 0; i < D; i++)
        items[i] = bvec_varfdd(i);

    //first item cannot be 0 (a)
    bvec zero = bvec_con(items[0].bitnum(), 0);
    bdd c1 = bvec_neq(items[0], zero);

    //(i = 1) and (j = 2) => i < j
    bdd c2 = bddtrue; 
    for (int i = 0; i < D - 1; i++) {
        int j = i + 1;
        bvec one = bvec_con(items[i].bitnum(), 1);
        bvec two = bvec_con(items[j].bitnum(), 2);
        bdd iequ = bvec_equ(items[i], one);
        bdd jequ = bvec_equ(items[j], two);
        bdd x = bdd_imp((iequ & jequ), bvec_lte(items[i], items[j]));
        c2 &= x;
    }

    bvec two = bvec_con(items[3].bitnum(), 2);
    bdd c3 = bvec_equ(items[3], two);
    

    bdd c = c1 & c2 & c3;

    printf("Number of satisfying arguments: %f \n", bdd_satcount(c));

    cout << fddset << bdd_fullsatone(c) << endl;


    bdd_done();

    return 0;
}
