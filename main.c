#include <stdio.h>
#include <stdint.h>
#include <conf.h>
#include <vptree.h>
#include <utils.h>
#include <sodium.h>

/* array of indices (to keep track of VP's original index */
uint32_t* DATASET_INDEX;

/// \brief Frees up all pointers allocated during the building process of the VP tree by recursively freeing subtrees.
/// \param vpTreePtr
void free_vpt( vptree* vpTreePtr )
{
    // Free inner / outer subtrees
    if ( NULL != vpTreePtr->inner )
        free_vpt( vpTreePtr->inner );

    if ( NULL != vpTreePtr->outer )
        free_vpt( vpTreePtr->outer );

    // Free pointers inside parent tree
    free( vpTreePtr->vp );

    // Free parent
    free( vpTreePtr );
}


int main()
{
    static double DATASET[DATASET_N][DATASET_D];
    vptree *vpTree;
    uint32_t i;
    uint32_t j;

    // Initialize Dataset
    for ( i = 0; i < DATASET_N; i++ )
        for ( j = 0; j < DATASET_D; j++ )
        {
//            DATASET[i][j] = ( randombytes_uniform( INT32_MAX ) / (double) INT32_MAX ) * ( randombytes_uniform( INT32_MAX ) / (double) INT32_MAX );
            DATASET[i][j] = i;
            fprintf( stdout, "DATASET[%u] = %lf\n", i, DATASET[i][j] );
        }

    // Initialize index
    DATASET_INDEX = (uint32_t *) malloc(DATASET_N * sizeof( uint32_t ) );
    for ( i = 0; i < DATASET_N; i++ )
        DATASET_INDEX[i] = i;

    // Construct VP Tree from dataset (SERIAL)
    vpTree = buildvp( *DATASET, DATASET_N, DATASET_D );

    fprintf( stdout, "\n\nfinished building tree...\n\n" );

    // Inspect VP Tree
    inspect_vpt( vpTree, 0 );

    // Free memory
    free_vpt( vpTree );
    free( DATASET_INDEX );

    return 0;
}