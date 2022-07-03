#ifndef __ETHER_MATRIX_H__
#define __ETHER_MATRIX_H__

#include "ether_defines.h"

EAPI void  ether_matrix_identity         (EtherMatrix m);
EAPI void  ether_matrix_multiply         (EtherMatrix result, EtherMatrix m1, EtherMatrix m2);
EAPI void  ether_matrix_inverse          (EtherMatrix result, EtherMatrix m);
EAPI void  ether_matrix_rot_x            (EtherMatrix m, EtherAngle angle, int leftside);
EAPI void  ether_matrix_rot_y            (EtherMatrix m, EtherAngle angle, int leftside);
EAPI void  ether_matrix_rot_z            (EtherMatrix m, EtherAngle angle, int leftside);
EAPI void  ether_matrix_rot_vector       (EtherMatrix m, EtherAngle angle, EtherVector vector, int leftside);
EAPI void  ether_matrix_reset_rotations  (EtherMatrix m);
EAPI void  ether_matrix_basis_get        (EtherVector v, EtherMatrix m, int axis);
EAPI void  ether_matrix_basis_set        (EtherMatrix m, EtherVector v, int axis);
EAPI void  ether_matrix_translate        (EtherMatrix result, EtherVector v, int leftside);
EAPI void  ether_matrix_rotations_get    (EtherMatrix m, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz);
EAPI void  ether_matrix_copy             (EtherMatrix dest, EtherMatrix source);
EAPI void  ether_matrix_translation_get  (EtherVector v, EtherMatrix m);
EAPI void  ether_matrix_translation_set  (EtherMatrix m, EtherVector v);

#endif /* __ETHER_MATRIX_H__ */
