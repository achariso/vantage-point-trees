#include <vptree.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <utils.h>
#include <math.h>
#include <string.h>

/// \brief Compute euclidean distance between two $d-dimensional points, A and B.
/// \param pointA
/// \param pointB
/// \param d The number of dimensions (columns of data points)
/// \return The euclidean distance, sqrt( Σ(Axi - Bxi)^2 )
double euclidean_distance( double* pointA, double* pointB, uint16_t d )
{
    double sum = 0.0;
    for ( uint32_t j = 0; j < d; j++ )
        sum += pow( pointA[j] - pointB[j], 2.0);

    return sqrt( sum );
}

/* array of indices (to keep track of VP's original index */
extern uint32_t* DATASET_INDEX;
//uint32_t* dataset_index_vpt = NULL;


// ========== LIST OF ACCESSORS

/// \brief Build vantage-point tree given input dataset X
/// \param X Input data points, stored as [n-by-d] array
/// \param n Number of data points (rows of X)
/// \param d Number of dimensions (columns of X)
/// \return The vantage-point tree
vptree* buildvp(double *X, int n, int d)
{
    // init VP Tree pointer
    vptree *vpTreePtr = (vptree *) malloc( sizeof( vptree ) );

    // save tree env
    vpTreePtr->dataset = X;
    vpTreePtr->dataset_rows = n - 1;    // 9
    vpTreePtr->dataset_dims = d;
    vpTreePtr->dataset_left = 0;
    vpTreePtr->dataset_right = n - 2;   // 8

//    vpTreePtr->index = &DATASET_INDEX[vpTreePtr->dataset_left];

    //======= start: building the tree

    //  1) Select Vantage Point
    vpTreePtr->vp = (double *) malloc( d * sizeof( double ) );
    memcpy( vpTreePtr->vp, getVP( vpTreePtr ), d * sizeof( double ) );
    vpTreePtr->vp_index = (uint32_t) getIDX( vpTreePtr );

    // 2) Compute distances between VP and every other point
    vpTreePtr->md = getMD( vpTreePtr );

    // recursively compute inner & outer sub-trees
    vpTreePtr->inner = getInner( vpTreePtr );
    vpTreePtr->outer = getOuter( vpTreePtr );

    //======= end: building the tree

    return vpTreePtr;
}

/// \brief Return vantage-point subtree with points inside radius
/// \param parentVpTreePtr A vantage-point tree
/// \return The vantage-point subtree
vptree* getInner(vptree* parentVpTreePtr)
{
    if ( parentVpTreePtr->md_index == 0 )
        return NULL;

    // init pointer
    vptree *vpTreePtr = (vptree *) malloc( sizeof( vptree ) );

    // save tree env
    vpTreePtr->dataset = parentVpTreePtr->dataset;
    vpTreePtr->dataset_rows = parentVpTreePtr->md_index;        // 4
    vpTreePtr->dataset_dims = parentVpTreePtr->dataset_dims;
    vpTreePtr->dataset_left = parentVpTreePtr->dataset_left;    // 0
    vpTreePtr->dataset_right = parentVpTreePtr->md_index - 1;   // 3

    vpTreePtr->inner = NULL;
    vpTreePtr->outer = NULL;

    fprintf( stdout, "\nX_inner:\n" );
    for (uint32_t i = vpTreePtr->dataset_left; i <= vpTreePtr->dataset_right; ++i) {
        fprintf( stdout, "dataset[%u] = %lf\n", i, vpTreePtr->dataset[i]  );
    }

    //======= start: building the inner tree

    //  1) Select Vantage Point
    vpTreePtr->vp = (double *) malloc( vpTreePtr->dataset_dims * sizeof( double ) );
    memcpy( vpTreePtr->vp, getVP( vpTreePtr ), vpTreePtr->dataset_dims * sizeof( double ) );
    vpTreePtr->vp_index = (uint32_t) getIDX( vpTreePtr );


//    inspect_vpt( vpTreePtr, 0 );

    // 2) Compute distances between VP and every other point
    if ( vpTreePtr->dataset_rows > 1 )
        vpTreePtr->md = getMD( vpTreePtr );
    else
        vpTreePtr->md = 0.0;

    // recursively compute inner & outer sub-trees
    if ( vpTreePtr->dataset_rows > 2 )
    {
        vpTreePtr->inner = getInner( vpTreePtr );
//        vpTreePtr->outer = getOuter( vpTreePtr );
    }
    else
    {
        vpTreePtr->inner = NULL;
        vpTreePtr->outer = NULL;
    }

    //======= end: building the inner tree

    return vpTreePtr;


}

/// \brief Return vantage-point subtree with points outside radius
/// \param node A vantage-point tree
/// \return The vantage-point subtree
vptree* getOuter(vptree* parentVpTreePtr)
{
    if ( ( parentVpTreePtr->dataset_rows - parentVpTreePtr->md_index ) <= 3 )
        return NULL;

    // init pointer
    vptree *vpTreePtr = (vptree *) malloc( sizeof( vptree ) );

    // save tree env
    vpTreePtr->dataset = parentVpTreePtr->dataset;
    vpTreePtr->dataset_rows = parentVpTreePtr->dataset_rows - parentVpTreePtr->md_index - 2;    // 4
    vpTreePtr->dataset_dims = parentVpTreePtr->dataset_dims;
    vpTreePtr->dataset_left = parentVpTreePtr->md_index;            // 5
    vpTreePtr->dataset_right = parentVpTreePtr->dataset_right - 1;  // 8

    //======= start: building the outer tree

    //  1) Select Vantage Point
    vpTreePtr->vp = (double *) malloc( vpTreePtr->dataset_dims * sizeof( double ) );
    memcpy( vpTreePtr->vp, getVP( vpTreePtr ), vpTreePtr->dataset_dims * sizeof( double ) );
    vpTreePtr->vp_index = (uint32_t) getIDX( vpTreePtr );

    // 2) Compute distances between VP and every other point
    if ( vpTreePtr->dataset_rows > 2 )
        vpTreePtr->md = getMD( vpTreePtr );
    else
        vpTreePtr->md = 0.0;

    // recursively compute inner & outer sub-trees
    if ( vpTreePtr->dataset_rows > 2 )
    {
        vpTreePtr->inner = getInner( vpTreePtr );
        vpTreePtr->outer = getOuter( vpTreePtr );
    }
    else
    {
        vpTreePtr->inner = NULL;
        vpTreePtr->outer = NULL;
    }

    //======= end: building the outer tree

    return vpTreePtr;
}

/// \brief Return median of distances to vantage point
/// \param T A vantage-point tree
/// \return The median distance
double getMD(vptree* T)
{
    uint32_t i;
    uint32_t distance_i;

    // re-format nodes dataset pointer
    double (*Xf)[T->dataset_dims] = (double (*)[T->dataset_dims]) ( T->dataset );

    // find distances of vp to all other
    double vpDistance[T->dataset_rows];
    for ( distance_i = 0, i = T->dataset_left; i <= T->dataset_right; distance_i++, i++ )
    {
        vpDistance[distance_i] = euclidean_distance(T->vp, &Xf[i][0], T->dataset_dims );
//        fprintf( stdout, "\t\tvpDistance[%u] = %lf\n", distance_i, vpDistance[distance_i] );
    }

    // find median distance using QuickSelect
    return vpMedianDistanceQuickSelect( T, vpDistance );
}

/// \brief Return the coordinates of the vantage point
/// \param T A vantage-point tree
/// \return The coordinates [d-dimensional vector]
double* getVP(vptree* T)
{
    double (*dataset)[T->dataset_dims] = (double (*)[T->dataset_dims]) T->dataset;
    return (double *) &dataset[T->dataset_right + 1];
}

/// \brief Return the index of the vantage point
/// \param T A vantage-point tree
/// \return
int getIDX(vptree* T)
{
    uint32_t idx = DATASET_INDEX[T->dataset_right + 1];
//    double (*dataset)[T->dataset_dims] = (double (*)[T->dataset_dims]) T->dataset;
//      //      - inspect
//    fprintf(stdout, "\nN = %u\n\tvp = DATASET[%u] = (", T->dataset_rows, idx );
//    for(uint32_t j = 0; j < T->dataset_dims; j++ )
//    {
//        fprintf( stdout, "%lf (%lf), ", T->vp[j], dataset[idx][j] );
//    }
//    fprintf( stdout, "\b\b )\n" );

    return idx;
}