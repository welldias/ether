#include <Ether.h>


static EtherFactor _ether_math_sin_table[258];

void 
ether_math_init(void)
{
  int i;
  /* Tricky stuff here:
   * Each entry in _ether_math_sin_table[] is 1/256th of a quarter-circle;
   * since a quarter circle is PI/2 radians, each entry is therefore
   * 1/256th of PI/2 radians.  We add a couple of entries at the end,
   * since we'll be interpolating between the two values that sandwich
   * our angle.
   */
  for (i = 0; i < 256; ++i)
    _ether_math_sin_table[i] = ETHER_FLOAT_TO_FACTOR(sin(i * ((PI/2) / 256)));
  _ether_math_sin_table[256] = _ether_math_sin_table[257] = ETHER_UNITY;
}

EtherScalar 
ether_math_scalar_abs (EtherScalar alpha)
{
  return (EtherScalar)fabs(alpha);
}

EtherScalar 
ether_math_scalar_round(EtherScalar alpha)
{
  return ETHER_FLOAT_TO_SCALAR(floor(ETHER_SCALAR_TO_FLOAT(alpha) + 0.5));
}

EtherFactor 
ether_math_scalar_divide(EtherScalar alpha, EtherScalar beta)
{
  return (EtherFactor)(((float)(alpha)) / ((float)(beta)));
}

EtherScalar 
ether_math_scalar_mult_div(EtherScalar alpha, EtherScalar beta, EtherScalar gamma)
{
	return (EtherScalar)((((float)(alpha)) * ((float)(beta))) / ((float)(gamma)));
}

EtherScalar 
ether_math_factor_multiply(EtherFactor alpha, EtherScalar beta)
{
	return (EtherScalar)(((float)(alpha)) * ((float)(beta)));
}

EtherFactor
ether_math_sine(EtherAngle angle)
{
	return ETHER_FLOAT_TO_FACTOR(sin((PI * ETHER_ANGLE_TO_FLOAT(angle)) / 180.0));
}

EtherFactor
ether_math_cosine(EtherAngle angle)
{
	return ether_math_sine(angle + ETHER_FLOAT_TO_ANGLE(90));
}


