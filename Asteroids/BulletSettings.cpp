#include "BulletSettings.h"
#include "ConfigurationFile.hpp"

namespace Asteroids
{

void BulletSettings::LoadFromConfigurationFile(const std::string& fileName)
{
	ConfigurationFile bulletSettings(fileName);

	sideCount				= bulletSettings.GetProperty<size_t>("sideCount");
	velocityDampening		= bulletSettings.GetProperty<float>("velocityDampening");
	velocityMagnitude		= bulletSettings.GetProperty<float>("velocityMagnitude");
	angularDampening		= bulletSettings.GetProperty<float>("angularDampening");
	angularVelocity.min		= bulletSettings.GetProperty<float>("minAngularVelocity");
	angularVelocity.max		= bulletSettings.GetProperty<float>("maxAngularVelocity");
	approximateRadius		= bulletSettings.GetProperty<float>("approximateRadius");
}

} // namespace Asteroids