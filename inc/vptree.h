#ifndef VPTREE_H
#define VPTREE_H

#include "conf.h"
#include <stdint.h>

// type definition
typedef struct vptree_t {
    // dataset for this VP tree
    double* dataset;
    uint32_t dataset_left;
    uint32_t dataset_right;
    uint16_t dataset_dims;
    uint16_t dataset_rows;

    // vantage point
    double* vp;

    // vp's original index in DATASET array
    uint32_t vp_index;

    // median distance of vp to all other points in each subset
    double md;
    uint32_t md_index;  // index of last median

    // vp' subtrees
    struct vptree_t *inner;
    struct vptree_t *outer;
} vptree;


/// \brief Compute euclidean distance between two $d-dimensional points, A and B.
/// \param pointA
/// \param pointB
/// \param d The number of dimensions (columns of data points)
/// \return The euclidean distance, sqrt( Σ(Axi - Bxi)^2 )
double euclidean_distance( double* pointA, double* pointB, uint16_t d );


// ========== LIST OF ACCESSORS

/// \brief Build vantage-point tree given input dataset X
/// \param X Input data points, stored as [n-by-d] array
/// \param n Number of data points (rows of X)
/// \param d Number of dimensions (columns of X)
/// \return The vantage-point tree
vptree* buildvp(double *X, int n, int d);

/// \brief Return vantage-point subtree with points inside radius
/// \param parentVpTreePtr A vantage-point tree
/// \return The vantage-point subtree
vptree* getInner(vptree* parentVpTreePtr);

/// \brief Return vantage-point subtree with points outside radius
/// \param node A vantage-point tree
/// \return The vantage-point subtree
vptree* getOuter(vptree* node);

/// \brief Return median of distances to vantage point
/// \param T A vantage-point tree
/// \return The median distance
double getMD(vptree* T);

/// \brief Return the coordinates of the vantage point
/// \param T A vantage-point tree
/// \return The coordinates [d-dimensional vector]
double* getVP(vptree* T);

/// \brief Return the index of the vantage point
/// \param T A vantage-point tree
/// \return 
int getIDX(vptree* T);

#endif //VPTREE_H
