#ifndef __ETHER_VECTOR_H__
#define __ETHER_VECTOR_H__

/*
 * A Ether_Vector is a three-element array, which can be 
 * indexed by the #defined constants X, Y and Z; for example, 
 * if v is a vector then v[X] is the X-component of the vector. 
 * In general, Ether_Vector are made up of three Ether_Scalars; 
 * however, a normalized vector (such as a facet normal, a basis 
 * vector, or a vector that's been normalized using the 
 * Ether_VectorNormalize() function) will actually have 
 * Ether_Factors as elements.
 */

typedef EtherScalar EtherVector[3];
extern EtherVector Ether_VectorNULL;

EAPI void          ether_vector_create          (EtherVector result, EtherScalar x, EtherScalar y, EtherScalar z);
EAPI void          ether_vector_add             (EtherVector result, EtherVector v1, EtherVector v2);
EAPI void          ether_vector_sub             (EtherVector result, EtherVector v1, EtherVector v2);
EAPI void          ether_vector_negative        (EtherVector v);
EAPI EtherFactor   ether_vector_dotproduct      (EtherVector v1, EtherVector v2);
EAPI EtherScalar   ether_vector_crossproduct    (EtherVector result, EtherVector v1, EtherVector v2);
EAPI EtherScalar   ether_vector_magnitude       (EtherVector v);
EAPI void          ether_vector_normalize       (EtherVector v);
EAPI EtherScalar   ether_vector_distance        (EtherVector v1, EtherVector v2);
EAPI void          ether_vector_scale           (EtherVector v, EtherScalar newmag);
EAPI void          ether_vector_rescale         (EtherVector v, EtherScalar newmag);
EAPI void          ether_vector_zero            (EtherVector v);
EAPI int           ether_vector_equal           (EtherVector v1, EtherVector v2);
EAPI EtherVector  *ether_vector_copy            (EtherVector dest, EtherVector source);

#endif /* __ETHER_VECTOR_H__ */
