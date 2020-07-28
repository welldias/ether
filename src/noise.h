#ifndef __ETHER_NOISE_H__
#define __ETHER_NOISE_H__

namespace ether {
	class SimpleNoise {
	public:
		SimpleNoise(int seed=0);
		~SimpleNoise();
		
		float Evaluate(const Vector3& point);

	private:
		void   Randomize(int seed);
		double Dot2(int g[], double x, double y);
		double Dot3(int g[], double x, double y, double z);
		double Dot4(int g[], double x, double y, double z, double t);
		int    FastFloor(double x);
		void   FastUnpackLittleUint32(int value, char* buffer, int sizeBuffer);

	private:
		static const int randomSize;
		static const int permutationTable[];
		static int noiseGrad3[12][3];

		int* randomTable = NULL;

		double F2;

		double G2, G22;
		double F3, F4;
		double G3, G4;
		double G42, G43, G44;
	};
}

#endif // __ETHER_NOISE_H__