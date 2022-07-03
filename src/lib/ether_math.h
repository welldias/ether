#ifndef __ETHER_MATH_H__
#define __ETHER_MATH_H__

#include "ether_defines.h"

EAPI void        ether_math_init            (void);
EAPI EtherScalar ether_math_scalar_abs      (EtherScalar alpha);
EAPI EtherScalar ether_math_scalar_round    (EtherScalar alpha);
EAPI EtherFactor ether_math_scalar_divide   (EtherScalar alpha, EtherScalar beta);
EAPI EtherScalar ether_math_scalar_mult_div (EtherScalar alpha, EtherScalar beta, EtherScalar gamma);
EAPI EtherScalar ether_math_factor_multiply (EtherFactor alpha, EtherScalar beta);
EAPI EtherFactor ether_math_sine            (EtherAngle angle);
EAPI EtherFactor ether_math_cosine          (EtherAngle angle);


#endif /* __ETHER_MATH_H__ */
