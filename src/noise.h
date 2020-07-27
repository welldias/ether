#ifndef __ETHER_NOISE_H__
#define __ETHER_NOISE_H__

namespace ether {

	void  ether_simple_noise_init(int seed);
	float ether_simple_noise_evaluate(vec3 point);
}

#endif // __ETHER_NOISE_H__