#include <random>

namespace Asteroids
{

int	Random(int min, int max)
{
	using IntDistribution = std::uniform_int_distribution<int>;

	static std::random_device	randomDevice;
	static std::mt19937			generator(randomDevice());
	static IntDistribution		distribution;

	distribution = IntDistribution(min, max);

	return distribution(generator);
}

float Random(float min, float max)
{
	using FloatDistribution = std::uniform_real_distribution<float>;

	static std::random_device	randomDevice;
	static std::mt19937			generator(randomDevice());
	static FloatDistribution	distribution;

	distribution = FloatDistribution(min, max);

	return distribution(generator);
}

} // namespace Asteroids