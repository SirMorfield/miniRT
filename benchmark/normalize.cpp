#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#define COUNT 400000000

double q_rsqrt(double number) {
	static_assert(std::numeric_limits<double>::is_iec559, "fast inverse square root requires IEEE-comliant 'double'");
	static_assert(sizeof(double) == sizeof(std::uint64_t), "fast inverse square root requires 'float' to be 64-bit");

	double	 x2 = number * 0.5;
	double	 y = number;
	uint64_t i;
	std::memcpy(&i, &y, sizeof(number));
	i = 0x5fe6eb50c7b537a9 - (i >> 1);
	std::memcpy(&y, &i, sizeof(number));

	return y * (1.5 - (x2 * y * y));
}

class Vec3 {
  public:
	Vec3() {}

	void normalize() {
		double invLen = 1 / std::sqrt(x * x + y * y + z * z);
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	void q_normalize() {
		double invLen = q_rsqrt(x * x + y * y + z * z);
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	double x;
	double y;
	double z;
};

int main() {
	std::vector<Vec3> vecs;
	vecs.resize(COUNT);

	{
		auto start = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < COUNT; i++) {
			vecs[i].normalize();
		}
		auto   elapsed = std::chrono::high_resolution_clock::now() - start;
		double ms_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		std::cout << "normalize(): " << ms_elapsed << " ms" << std::endl;
	}
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < COUNT; i++) {
			vecs[i].q_normalize();
		}
		auto   elapsed = std::chrono::high_resolution_clock::now() - start;
		double ms_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		std::cout << "q_normalize(): " << ms_elapsed << " ms" << std::endl;
	}

	return 0;
}
