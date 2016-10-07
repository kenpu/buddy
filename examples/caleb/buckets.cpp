/**
 * How many different wasy can X unique items be distributed 
 * amongst Y buckets, each with capacity Z.
 **/

#include <stdio.h>
#include "bdd.h"
#include "fdd.h"
#include "bvec.h"

using namespace std;

/**
 * Returns true if the current bucket is at or below the bucket capacity.
 *
 * items - array of bvecs to check
 * length - lengths of items
 * bucket - the current bucket we are checking for capacity
 * bucket_capacity - maximum capacity allowed for buckets
 **/
bdd is_below_capacity(bvec *items, int length, int bucket, int bucket_capacity) {
    bdd c = bddtrue;
    bvec count = bvec_con((items[0]).bitnum(), 0);
    for (int i = 0; i < length; i++) {
        bvec item = items[i];
        bvec bucket_vec = bvec_con(item.bitnum(), bucket);
        if (bddtrue == bvec_equ(bucket_vec, item)) {
            count++;
        }
    }
    if (count <= bucket_capacity) {
        return bddtrue;
    } else {
        return bddfalse;
    }
}

int main(int argc, char **argv) {

    bdd_init(10000, 10000);

    int X = 2; //Number of unique items
    int Y = 2;  //Number of buckets
    int Z = 2;  //Bucket capacity

    int domains[X];

    fill_n(domains, X, Y);

    fdd_extdomain(domains, X);

    //Create list of bvec items
    bvec *items = new bvec[X];

    for (int i = 0; i < X; i++) {
        items[i] = bvec_varfdd(i); 
    }

    //Create bucket conditions
    bdd *conditions = new bdd[Y];
    
    for (int i = 0; i < Y; i++) {
        conditions[i] = is_below_capacity(items, X, i, Z);
    }

    bdd c = bddtrue;

    for (int i = 0; i < Y; i++) {
        c &= conditions[i];
    }

    printf("# of solutions is: %ld\n", (long)bdd_satcount(c));

    cout << fddset << c << endl;

    //Cleanup
    delete [] items;
    delete [] conditions;

    return 0;
}
