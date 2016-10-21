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

    bdd_init(10000000, 10000000);

    int X = 10;  //Number of unique items
    int Y = 10;  //Number of buckets
    int Z = 1;  //Bucket capacity

    int domains[X*Y];

    fill_n(domains, X*Y, 2);

    fdd_extdomain(domains, X*Y);

    //Create a matrix of bvec items
    
    bvec **items = new bvec *[X];
    for(int i=0; i < X; i++)
        items[i] = new bvec[Y];

    for (int i = 0; i < X; i++)
        for(int j=0; j < Y; j++)
            items[i][j] = bvec_varfdd(i * Y + j); 

    int n = items[0][0].bitnum();
    int N = 5;

    // bucket size bvec

    bvec *sizes = new bvec[Y];

    for(int j=0; j < Y; j++) {
        sizes[j] = bvec_coerce(N, items[0][j]);
        for(int i = 1; i < X; i++)
            sizes[j] = sizes[j] + bvec_coerce(N, items[i][j]);
    }

    // bucket count bvec

    bvec *counts = new bvec[X];

    for(int i=0; i < X; i++) {
        counts[i] = bvec_coerce(N, items[i][0]);
        for(int j=1; j < Y; j++)
            counts[i] = counts[i] + bvec_coerce(N, items[i][j]);
    }

    // all counts are = 1
    bdd c1 = bddtrue;
    bvec one = bvec_con(N, 1);
    for(int i = 0; i < X; i++) {
        c1 &= (counts[i] == one);
    }

    // all sizes are <= Z
    bdd c2 = bddtrue;
    for(int j=0; j < Y; j++)
        c2 &= bvec_lte(sizes[j], bvec_con(N, Z));

    bdd c = c1 & c2;

    printf("# of solutions is: %ld\n", (long)bdd_satcount(c));

    // cout << fddset << c << endl;
    bdd sol = bdd_satone(c);

    return 0;
}
