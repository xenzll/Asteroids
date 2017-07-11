#pragma once
#include <string>

namespace Asteroids
{

struct BulletSettings
{
	void LoadFromConfigurationFile(const std::string& fileName);

	size_t	sideCount;
	float	velocityDampening;
	float	angularDampening;
	float	velocityMagnitude;
	float	approximateRadius;

	struct { float min, max; } angularVelocity;
};

} // namespace Asteroids