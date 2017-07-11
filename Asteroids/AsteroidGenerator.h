#pragma once
#include "Asteroid.h"

#include <memory>

namespace Asteroids
{

class AsteroidGenerator
{

public:

	using AsteroidPtr = std::unique_ptr<Asteroid>;

	AsteroidGenerator()  = default;
	~AsteroidGenerator() = default;

	AsteroidPtr		GenerateAsteroid(AsteroidSize size);
	AsteroidSize	GenerateRandomAsteroidSize();

private:

	void	GenerateVertices(AsteroidPtr& asteroid, AsteroidSize size);
	void	GeneratePhyscicalProperties(AsteroidPtr& asteroid);
	float	GenerateRandomRadius(AsteroidSize size);
};

} // namespace Asteroids