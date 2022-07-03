#include "ether_matrix.h"
#include "ether_private.h"
#include "ether_math.h"
#include "ether_vector.h"

#define _ETHER_EPSILON 0.000001
   
static void _ether_matrix_mult3x3(EtherMatrix result, EtherMatrix m1, EtherMatrix m2);

void
ether_matrix_identity(EtherMatrix m)
{
  ether_matrix_reset_rotations(m);
  m[_ETHER_TRANS][X] = m[_ETHER_TRANS][Y] = m[_ETHER_TRANS][Z] = 0;
}

void
ether_matrix_multiply(EtherMatrix result, EtherMatrix m1, EtherMatrix m2)
{
  _ether_matrix_mult3x3(result, m1, m2);
  _ether_transform(result[_ETHER_TRANS], m1, m2[_ETHER_TRANS]);
}
    
void
ether_matrix_inverse(EtherMatrix result, EtherMatrix m)
{
  /* transpose the rotation part */
  result[0][0] = m[0][0];  result[0][1] = m[1][0];  result[0][2] = m[2][0];
  result[1][0] = m[0][1];  result[1][1] = m[1][1];  result[1][2] = m[2][1];
  result[2][0] = m[0][2];  result[2][1] = m[1][2];  result[2][2] = m[2][2];

  /* transform the translation part */
  result[_ETHER_TRANS][X] = -ether_vector_dotproduct(result[X], m[_ETHER_TRANS]);
  result[_ETHER_TRANS][Y] = -ether_vector_dotproduct(result[Y], m[_ETHER_TRANS]);
  result[_ETHER_TRANS][Z] = -ether_vector_dotproduct(result[Z], m[_ETHER_TRANS]);
}

void
ether_matrix_rot_x(EtherMatrix m, EtherAngle angle, int leftside)
{
  EtherMatrix s, t;
  ether_matrix_identity(s);
  s[2][2] =   s[1][1] = ether_math_cosine(angle);
  s[1][2] = -(s[2][1] = ether_math_sine(angle));
  ether_matrix_copy(t, m);
  if (leftside)
    _ether_matrix_mult3x3(m, s, t);
  else
    _ether_matrix_mult3x3(m, t, s);
}

void
ether_matrix_rot_y(EtherMatrix m, EtherAngle angle, int leftside)
{
  EtherMatrix s, t;
  ether_matrix_identity(s);
  s[2][2] =   s[0][0] = ether_math_cosine(angle);
  s[2][0] = -(s[0][2] = ether_math_sine(angle));
  ether_matrix_copy(t, m);
  if (leftside)
    _ether_matrix_mult3x3(m, s, t);
  else
    _ether_matrix_mult3x3(m, t, s);   
}

void
ether_matrix_rot_z(EtherMatrix m, EtherAngle angle, int leftside)
{
  EtherMatrix s, t;
  ether_matrix_identity(s);
  s[1][1] =   s[0][0] = ether_math_cosine(angle);
  s[0][1] = -(s[1][0] = ether_math_sine(angle));
  ether_matrix_copy(t, m);
  if (leftside)
    _ether_matrix_mult3x3(m, s, t);
  else
    _ether_matrix_mult3x3(m, t, s);
}

void
ether_matrix_rot_vector(EtherMatrix m, EtherAngle angle, EtherVector vector, int leftside)
{
  EtherMatrix tmp1, tmp2;
  EtherFactor s = ether_math_sine(-angle);
  EtherFactor c = ether_math_cosine(-angle);
  EtherFactor t = (EtherFactor)ETHER_UNITY - c;
  EtherFactor x = vector[X], y = vector[Y], z = vector[Z];
  tmp1[0][0] = ether_math_factor_multiply(ether_math_factor_multiply(t, x), x) + c;
  tmp1[0][1] = ether_math_factor_multiply(ether_math_factor_multiply(t, x), y) + ether_math_factor_multiply(s, z);
  tmp1[0][2] = ether_math_factor_multiply(ether_math_factor_multiply(t, x), z) - ether_math_factor_multiply(s, y);
  tmp1[1][0] = ether_math_factor_multiply(ether_math_factor_multiply(t, x), y) - ether_math_factor_multiply(s, z);
  tmp1[1][1] = ether_math_factor_multiply(ether_math_factor_multiply(t, y), y) + c;
  tmp1[1][2] = ether_math_factor_multiply(ether_math_factor_multiply(t, y), z) + ether_math_factor_multiply(s, x);
  tmp1[2][0] = ether_math_factor_multiply(ether_math_factor_multiply(t, x), z) + ether_math_factor_multiply(s, y);
  tmp1[2][1] = ether_math_factor_multiply(ether_math_factor_multiply(t, y), z) - ether_math_factor_multiply(s, x);
  tmp1[2][2] = ether_math_factor_multiply(ether_math_factor_multiply(t, z), z) + c;
  ether_matrix_copy(tmp2, m);
  if (leftside)
    _ether_matrix_mult3x3(m, tmp1, tmp2);
  else
    _ether_matrix_mult3x3(m, tmp2, tmp1);
}

void
ether_matrix_reset_rotations(EtherMatrix m)
{
  memset(m, 0, 9 * sizeof(EtherFactor));
  m[0][0] = m[1][1] = m[2][2] = ETHER_UNITY;
}

void
ether_matrix_basis_get(EtherVector v, EtherMatrix m, int axis)
{
  v[X] = m[X][axis];
  v[Y] = m[Y][axis];
  v[Z] = m[Z][axis];
}

void
ether_matrix_basis_set(EtherMatrix m, EtherVector v, int axis)
{
  m[X][axis] = v[X];
  m[Y][axis] = v[Y];
  m[Z][axis] = v[Z];
}

void
ether_matrix_translate(EtherMatrix result, EtherVector v, int leftside)
{
  if (leftside)
  {
    result[_ETHER_TRANS][X] += v[X];
    result[_ETHER_TRANS][Y] += v[Y];
    result[_ETHER_TRANS][Z] += v[Z];
  }
  else
  {
    EtherVector vtmp;
    _ether_transform(vtmp, result, v);
    ether_vector_copy(result[_ETHER_TRANS], vtmp);
  }
}

void
ether_matrix_rotations_get(EtherMatrix m, EtherAngle *rx, EtherAngle *ry, EtherAngle *rz)
{
  float m00 = ETHER_FACTOR_TO_FLOAT(m[0][0]);
  float m01 = ETHER_FACTOR_TO_FLOAT(m[0][1]);
  float m10 = ETHER_FACTOR_TO_FLOAT(m[1][0]);
  float m11 = ETHER_FACTOR_TO_FLOAT(m[1][1]);
  float m20 = ETHER_FACTOR_TO_FLOAT(m[2][0]);
  float m21 = ETHER_FACTOR_TO_FLOAT(m[2][1]);
  float m22 = ETHER_FACTOR_TO_FLOAT(m[2][2]);
  if((1.0 - fabs(m21)) < _ETHER_EPSILON)
  {
    if(rx)
      *rx = ETHER_FLOAT_TO_ANGLE(((m21 < 0) ? 90 : -90));
    if(ry)
      *ry = ETHER_FLOAT_TO_ANGLE(0.0);
    if(rz)
      *rz = ETHER_FLOAT_TO_ANGLE(-atan2(m10, m00) * 180 / PI);
  }
  else
  {
    float rotz, a;
    if(ry)
      *ry = ETHER_FLOAT_TO_ANGLE(atan2(-m20, m22) * 180 / PI);
    if(rz)
      *rz = ETHER_FLOAT_TO_ANGLE(atan2(-m01, m11) * 180 / PI);
    rotz = (float)atan2(-m01, m11);
    if(fabs(fabs(rotz) - PI / 2.0) < _ETHER_EPSILON)
      a =  m01 / (float)sin(rotz);
    else
      a = m11 / (float)cos(rotz);
    if(rx)
      *rx = - ETHER_FLOAT_TO_ANGLE(atan2(-m21, a) * 180 / PI);
  }
}

void
ether_matrix_copy(EtherMatrix dest, EtherMatrix source)
{
  memcpy((dest), (source), sizeof(EtherMatrix));
}

void
ether_matrix_translation_get(EtherVector v, EtherMatrix m)
{
  ether_vector_copy(v, m[3]);
}

void
ether_matrix_translation_set(EtherMatrix m, EtherVector v)
{
  ether_vector_copy(m[3], v);
}

#define _ETHER_FACTOR_PRODUCT(f1, f2)      (ETHER_FACTOR_TO_FLOAT(f1) * ETHER_FACTOR_TO_FLOAT(f2))

static void 
_ether_matrix_mult3x3(EtherMatrix result, EtherMatrix m1, EtherMatrix m2)
{
  int i, j;
  for (i = 0; i < 3; ++i)
    for (j = 0; j < 3; ++j)
      result[i][j] = ETHER_FLOAT_TO_FACTOR(
          _ETHER_FACTOR_PRODUCT(m1[i][0], m2[0][j]) +
          _ETHER_FACTOR_PRODUCT(m1[i][1], m2[1][j]) +
          _ETHER_FACTOR_PRODUCT(m1[i][2], m2[2][j]) );
}


