#include "ether_defines.h"
#include "ether_private.h"
#include "ether_vector.h"

/*
 * You should never have to use any of the transform functions directly; this 
 * is all handled for you by Ether.  A vector can be transformed by a matrix, 
 * or each component of the transform (X, Y or Z) can be computed separately.
 * 
 */

void _ether_transform(EtherVector result, EtherMatrix m, EtherVector v) {
    result[X] = _ether_transform_x(m, v);
    result[Y] = _ether_transform_y(m, v);
    result[Z] = _ether_transform_z(m, v);
}

EtherScalar _ether_transform_x(EtherMatrix m, EtherVector v) {
    return ether_vector_dotproduct(m[X], v) + m[_ETHER_TRANS][X];
}

EtherScalar _ether_transform_y(EtherMatrix m, EtherVector v) {
    return ether_vector_dotproduct(m[Y], v) + m[_ETHER_TRANS][Y];
}

EtherScalar _ether_transform_z(EtherMatrix m, EtherVector v) {
    return ether_vector_dotproduct(m[Z], v) + m[_ETHER_TRANS][Z];
}
