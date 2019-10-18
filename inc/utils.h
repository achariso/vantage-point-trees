#ifndef UTILS_H
#define UTILS_H

    #include <utils.h>
    #include <vptree.h>

    /// \brief Inspects given VP Tree and its subtrees
    /// \param vpTree
    /// \param level
    void inspect_vpt( vptree* vpTree, uint16_t level );

    /// \brief Swaps two pointers, i.e. two rows of a 2D array, of double elements.
    /// \param rowI Pointer to first row (or pointer to array of doubles)
    /// \param rowJ Pointer to second row (or pointer to array of doubles)
    void swap_double_ptr( double **rowI, double **rowJ );

    /// \brief Partition using Lomuto partition scheme
    /// \param distances Distance matrix of VP and every other point.
    /// \param left
    /// \param right
    /// \param pivotIndex
    /// \param vpTreePtr
    /// \return The pivot index
    uint32_t partition( double *distances, uint32_t left, uint32_t right, uint32_t pivotIndex, vptree *vpTreePtr );

    ///\brief Returns the k-th smallest element of list within left..right (i.e. left <= k <= right).
    /// The search space within the array is changing for each round - but the list is still the same size.
    /// Thus, k does not need to be updated with each round.
    /// \param distances
    /// \param left
    /// \param right
    /// \param k
    /// \param vpTreePtr
    /// \return
    double quickSelect( double *distances, uint32_t left, uint32_t right, uint32_t k, vptree *vpTreePtr );

    /// \brief Computes median distance of VP point to others using QuickSelect.
    /// \param vpTreePtr
    /// \param vpDistance
    /// \return
    double vpMedianDistanceQuickSelect( vptree* vpTreePtr, double *vpDistance );

#endif //UTILS_H
