#pragma once
#include <string>

namespace Asteroids
{

using AsteroidSizeType = int;

enum AsteroidSize : AsteroidSizeType
{
	Small,
	Medium,
	Large,
	Count
};

struct AsteroidDimensions
{
	size_t	vertexCount;
	struct { float min, max; } radius;
};

struct AsteroidGeneratorSettings
{
	void LoadFromConfigurationFile(const std::string& fileName);

	struct { float min, max; }	velocity;
	struct { float min, max; }	velocityDampening;
	struct { float min, max; }	angularVelocity;
	struct { float min, max; }	angularDampening;

	// Dimensions for each asteroid size
	AsteroidDimensions	dimensions[AsteroidSize::Count]; 
};

} // Asteroids