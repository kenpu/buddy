/**
 * How many different wasy can X unique items be distributed 
 * amongst Y buckets, each with capacity Z.
 **/

#include <stdio.h>
#include "bdd.h"
#include "fdd.h"
#include "bvec.h"

using namespace std;

int main(int argc, char **argv) {

    bdd_init(10000, 10000);

    int X = 2;  //Number of unique items
    int Y = 2;  //Number of buckets
    int Z = 1;  //Bucket capacity

    int domains[X*Y];

    fill_n(domains, X*Y, 100);

    fdd_extdomain(domains, X*Y);

    //Create a matrix of bvec items
    
    bvec **items = new bvec *[X];
    for(int i=0; i < X; i++)
        items[i] = new bvec[Y];

    for (int i = 0; i < X; i++)
        for(int j=0; j < Y; j++)
            items[i][j] = bvec_varfdd(i * Y + j); 

    int nbits = items[0][0].bitnum();
    bvec one = bvec_con(nbits, 1);

    // bucket size bvec

    bvec *sizes = new bvec[Y];

    for(int j=0; j < Y; j++) {
        sizes[j] = items[0][j];
        for(int i = 1; i < X; i++)
            sizes[j] = sizes[j] + items[i][j];
    }

    // bucket count bvec

    bvec *counts = new bvec[X];

    for(int i=0; i < X; i++) {
        counts[i] = items[i][0];
        for(int j=1; j < Y; j++)
            counts[i] = counts[i] + items[i][j];
    }

    // all counts are = 1
    bdd c1 = bddtrue;
    for(int i = 0; i < X; i++) {
        c1 &= (counts[i] == one);
    }

    // all sizes are <= Z
    bdd c2 = bddtrue;
    for(int j=0; j < Y; j++)
        c2 &= bvec_lte(sizes[j], bvec_con(nbits, Z));

    // items[i][j] <= 1
    bdd c3 = bddtrue;
    for(int i=0; i < X; i++)
        for(int j=0; j < Y; j++)
            c3 &= bvec_lte(items[i][j], bvec_con(nbits, 1));

    bdd c = c1 & c2 & c3;

    printf("# of solutions is: %ld\n", (long)bdd_satcount(c));

    cout << fddset << c << endl;

    return 0;
}
