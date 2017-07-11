#pragma once
#include <string>

namespace Asteroids
{

struct ShipSettings
{
	void LoadFromConfigurationFile(const std::string& fileName);

	float angularDampening;
	float velocityDampening;
	float approximateRadius;
	float rotationVelocity;
	float thrustMagnitude;	
};

} // namespace Asteroids