#include <utils.h>
#include <stdio.h>
#include <math.h>

/* array of indices (to keep track of VP's original index */
extern uint32_t* DATASET_INDEX;

/// \brief Inspects given VP Tree and its subtrees
/// \param vpTree
/// \param level
void inspect_vpt( vptree* vpTree, uint16_t level )
{
    char tabs[100];
    uint32_t i;
    uint32_t j;

    // format tabs
    for ( i = 0; i < level; i++ ) tabs[i] = '\t';
    tabs[i] = '\0';

    // inspect tree
    //  - N, D
    fprintf( stdout, "%s- left = %u, right = %u, rows = %u\n", tabs, vpTree->dataset_left, vpTree->dataset_right, vpTree->dataset_rows );
    //  - dataset
    if ( NULL != vpTree->dataset )
    {
        fprintf( stdout, "%s- DATASET = [\n", tabs );
        for ( i = vpTree->dataset_left; i <= vpTree->dataset_right ; i++)
            fprintf( stdout, "%s\t%lf\n", tabs, vpTree->dataset[i] );
        fprintf( stdout, "%s  ]\n", tabs );
    }
    //  - vp
    fprintf( stdout, "%s- vp = ( ", tabs );
    for( j = 0; j < DATASET_D; j++ )
    {
        fprintf( stdout, "%lf, ", vpTree->vp[j] );
    }
    fprintf( stdout, "\b\b )\n" );
    //  - vp index
    fprintf( stdout, "%s- vp_index = %u\n", tabs, vpTree->vp_index );
    //  - md
    fprintf( stdout, "%s- md = %lf\n", tabs, vpTree->md );
    //  - md index
    fprintf( stdout, "%s- md_index = %u\n", tabs, vpTree->md_index );

    // inspect subtrees
    if ( vpTree->inner != NULL )
    {
        //  - inner
        fprintf( stdout, "%s- INNER\n", tabs );
        inspect_vpt( vpTree->inner, level + 1 );

        if ( vpTree->outer != NULL )
        {
            //  - outer
            fprintf( stdout, "%s- OUTER\n", tabs );
            inspect_vpt( vpTree->outer, level + 1 );
        }
    }
}

/// \brief Swaps two pointers, i.e. two rows of a 2D array, of double elements.
/// \param rowI Pointer to first row (or pointer to array of doubles)
/// \param rowJ Pointer to second row (or pointer to array of doubles)
inline void swap_double_ptr( double **rowI, double **rowJ )
{
    double *tempPtr = *rowI;
    *rowI = *rowJ;
    *rowJ = tempPtr;
}

/// \brief Extends partition's swap operation to VP Tree's dataset as well as (global) $DATASET_INDEX.
/// \param vpTreePtr The VP Tree
/// \param indexI The first index in distance matrix (in range 0...N-1)
/// \param indexJ The second index in distance matrix (in range 0...N-1)
void vpSwap( vptree *vpTreePtr, uint32_t indexI, uint32_t indexJ )
{
#define SWAP_UI(x, y) { uint32_t temp = DATASET_INDEX[x]; DATASET_INDEX[x] = DATASET_INDEX[y]; DATASET_INDEX[y] = temp; }

    // Get real indices (in original dataset)
    // --------------------------------------
    // In QuickSelect of Distance matrix the initial bounds are 0...N-1, whereas in the original dataset that was
    // processed by this subtree the bounds are $dataset_left...$dataset_right. Thus, we need to adjust every swap in
    // Distance indices to a swap using indices of the original matrices $DATASET & $DATASET_INDEX.
    indexI += vpTreePtr->dataset_left;
    indexJ += vpTreePtr->dataset_left;

    // swap data points
    //  - re-format dataset pointer
    double (*Xf)[vpTreePtr->dataset_dims] = (double (*)[vpTreePtr->dataset_dims]) vpTreePtr->dataset;
    //  - do the swap
    swap_double_ptr( (double **) &Xf[indexI], (double **) &Xf[indexJ] );

    // swap indices
    SWAP_UI( indexI, indexJ )
}

/// \brief Partition using Lomuto partition scheme
/// \param distances Distance matrix of VP and every other point.
/// \param left
/// \param right
/// \param pivotIndex
/// \param vpTreePtr
/// \return The pivot index
uint32_t partition( double *distances, uint32_t left, uint32_t right, uint32_t pivotIndex, vptree *vpTreePtr )
{
#define SWAP_D(x, y) { double temp = x; x = y; y = temp; }

    // Pick pivotIndex as pivot from the array
    double pivot = distances[pivotIndex];

    // Move pivot to end
    SWAP_D(distances[pivotIndex], distances[right] )
    vpSwap( vpTreePtr, pivotIndex, right );

    // elements less than pivot will be pushed to the left of pIndex
    // elements more than pivot will be pushed to the right of pIndex
    // equal elements can go either way
    uint32_t pIndex = left;
    uint32_t i;

    // each time we finds an element less than or equal to pivot, pIndex
    // is incremented and that element would be placed before the pivot.
    for (i = left; i < right; i++)
    {
        if (distances[i] <= pivot)
        {
            SWAP_D(distances[i], distances[pIndex])
            vpSwap( vpTreePtr, i, pIndex );
            pIndex++;
        }
    }

    // Move pivot to its final place
    SWAP_D(distances[pIndex], distances[right])
    vpSwap( vpTreePtr, pIndex, right );

    // return pIndex (index of pivot element)
    return pIndex;
}

///\brief Returns the k-th smallest element of list within left..right (i.e. left <= k <= right).
/// The search space within the array is changing for each round - but the list is still the same size.
/// Thus, k does not need to be updated with each round.
/// \param distances
/// \param left
/// \param right
/// \param k
/// \param vpTreePtr
/// \return
double quickSelect( double *distances, uint32_t left, uint32_t right, uint32_t k, vptree *vpTreePtr )
{
    uint32_t pivotIndex;

    // If the array contains only one element, return that element
    if ( left == right )
        return distances[left];

    // select a pivotIndex between left and right
    pivotIndex = partition( distances, left, right, right, vpTreePtr );

    // The pivot is in its final sorted position
    if ( k == pivotIndex )
        return distances[k];


    // if k is less than the pivot index recurse on the left else on the right subarray
    return ( k < pivotIndex ) ?
           quickSelect(distances, left, pivotIndex - 1, k, vpTreePtr ) :
           quickSelect(distances, pivotIndex + 1, right, k, vpTreePtr );
}

/// \brief Computes median distance of VP point to others using QuickSelect.
/// \param vpTreePtr
/// \param vpDistance
/// \return
double vpMedianDistanceQuickSelect( vptree* vpTreePtr, double *vpDistance )
{
    double median;

    // compute median distance
    vpTreePtr->md_index = vpTreePtr->dataset_left + ( vpTreePtr->dataset_rows >> (unsigned)1 );
    median = quickSelect( vpDistance, 0, vpTreePtr->dataset_rows - 1, vpTreePtr->md_index - vpTreePtr->dataset_left, vpTreePtr );

    // move index so as to include all points with equal distance as the median
    while(vpDistance[ vpTreePtr->md_index - vpTreePtr->dataset_left ] == median )
        vpTreePtr->md_index++;

    vpTreePtr->md_index--;

    // return median distance
    return median;
}