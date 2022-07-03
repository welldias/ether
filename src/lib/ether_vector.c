#include <math.h>
#include <string.h>

#include "ether_vector.h"
#include "ether_math.h"

#define _ETHER_SCALAR_PRODUCT(s1, s2)      (ETHER_SCALAR_TO_FLOAT(s1) * ETHER_SCALAR_TO_FLOAT(s2))
#define _ETHER_SCALAR_FACTOR_PRODUCT(s, f) (ETHER_SCALAR_TO_FLOAT(s)  * ETHER_FACTOR_TO_FLOAT(f))

void ether_vector_create(EtherVector result, EtherScalar x, EtherScalar y, EtherScalar z)
{
	result[X] = x;  result[Y] = y;  result[Z] = z;
}

void 
ether_vector_add(EtherVector result, EtherVector v1, EtherVector v2)
{
	result[X] = v1[X] + v2[X];
	result[Y] = v1[Y] + v2[Y];
	result[Z] = v1[Z] + v2[Z];
}

void 
ether_vector_sub(EtherVector result, EtherVector v1, EtherVector v2)
{
	result[X] = v1[X] - v2[X];
	result[Y] = v1[Y] - v2[Y];
	result[Z] = v1[Z] - v2[Z];
}

void 
ether_vector_negative(EtherVector v)
{
	v[X] = -v[X];
	v[Y] = -v[Y];
	v[Z] = -v[Z];
}

EtherFactor 
ether_vector_dotproduct(EtherVector v1, EtherVector v2)
{
	return ETHER_FLOAT_TO_SCALAR(
		_ETHER_SCALAR_FACTOR_PRODUCT(v1[X], v2[X]) +
		_ETHER_SCALAR_FACTOR_PRODUCT(v1[Y], v2[Y]) + 
		_ETHER_SCALAR_FACTOR_PRODUCT(v1[Z], v2[Z]) );
}

EtherScalar 
ether_vector_crossproduct(EtherVector result, EtherVector v1, EtherVector v2)
{
	float xf, yf, zf, m;

	xf = _ETHER_SCALAR_PRODUCT(v1[Y], v2[Z]) - _ETHER_SCALAR_PRODUCT(v2[Y], v1[Z]);
	yf = _ETHER_SCALAR_PRODUCT(v2[X], v1[Z]) - _ETHER_SCALAR_PRODUCT(v1[X], v2[Z]);
	zf = _ETHER_SCALAR_PRODUCT(v1[X], v2[Y]) - _ETHER_SCALAR_PRODUCT(v2[X], v1[Y]);
	
	m = (float)sqrt(xf * xf + yf * yf + zf * zf);
	if (m == 0) 
		xf = yf = zf = 0;
	else {
		xf /= m;  yf /= m;  zf /= m;
	}
	result[X] = ETHER_FLOAT_TO_FACTOR(xf);
	result[Y] = ETHER_FLOAT_TO_FACTOR(yf);
	result[Z] = ETHER_FLOAT_TO_FACTOR(zf);
	return ETHER_FLOAT_TO_SCALAR(m);
}

EtherScalar 
ether_vector_magnitude(EtherVector v)
{
	float m = _ETHER_SCALAR_PRODUCT(v[X], v[X]) + 
		_ETHER_SCALAR_PRODUCT(v[Y], v[Y]) + 
		_ETHER_SCALAR_PRODUCT(v[Z], v[Z]);

	return ETHER_FLOAT_TO_SCALAR(sqrt(m));
}

void
ether_vector_normalize(EtherVector v)
{
	EtherScalar mag = ether_vector_magnitude(v);
	v[X] = ether_math_scalar_divide(v[X], mag);
	v[Y] = ether_math_scalar_divide(v[Y], mag);
	v[Z] = ether_math_scalar_divide(v[Z], mag);
}

EtherScalar 
ether_vector_distance(EtherVector v1, EtherVector v2)
{
	EtherVector v;
	ether_vector_sub(v, v1, v2);
	return ether_vector_magnitude(v);
}

void
ether_vector_scale(EtherVector v, EtherScalar newmag)
{
	v[X] = ether_math_factor_multiply(v[X], newmag);
	v[Y] = ether_math_factor_multiply(v[Y], newmag);
	v[Z] = ether_math_factor_multiply(v[Z], newmag);
}

void
ether_vector_rescale(EtherVector v, EtherScalar newmag)
{
	ether_vector_normalize(v);
	ether_vector_scale(v, newmag);
}

void 
ether_vector_zero(EtherVector v)
{
	v[X] = v[Y] = v[Z] = 0;
}

int 
ether_vector_equal(EtherVector v1, EtherVector v2)
{
  if(v1[X] == v2[X] && v1[Y] == v2[Y] && v1[Z] == v2[Z])
	  return 1;

  return 0;
}

EtherVector *
ether_vector_copy(EtherVector dest, EtherVector source)
{
    return memcpy((dest), (source), sizeof(EtherVector));
}
