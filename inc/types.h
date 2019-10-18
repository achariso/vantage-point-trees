#ifndef INC_1___PTHREADS___VANTAGE_POINT_TREES_TYPES_H
#define INC_1___PTHREADS___VANTAGE_POINT_TREES_TYPES_H

#include <conf.h>
#include <stdint.h>

typedef struct VPTree_t {

    // vantage point
    const double (*vp)[DATASET_D];

    // vp's original index in DATASET array
    uint32_t idx;

    // median distance of vp to all other points in each subset
    double md;

    // vp' subtrees
    struct VPTree_t *inner;
    struct VPTree_t *outer;

} VPTree;

#endif //INC_1___PTHREADS___VANTAGE_POINT_TREES_TYPES_H
