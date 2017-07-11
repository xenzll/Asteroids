#include "ObjectRemnantSettings.h"
#include "ConfigurationFile.hpp"

namespace Asteroids
{

void ObjectRemnantSettings::LoadFromConfigurationFile(const std::string& fileName)
{

	ConfigurationFile file(fileName);

	angularDampening.min	= file.GetProperty<float>("minAngularDampening");
	angularDampening.max	= file.GetProperty<float>("maxAngularDampening");
	lifeTime.min			= file.GetProperty<float>("minLifeTime");
	lifeTime.max			= file.GetProperty<float>("maxLifeTime");
	velocityX.min			= file.GetProperty<float>("minVelocity.x");
	velocityX.max			= file.GetProperty<float>("maxVelocity.x");
	velocityY.min			= file.GetProperty<float>("minVelocity.y");
	velocityY.max			= file.GetProperty<float>("maxVelocity.y");
	velocityDampening.min	= file.GetProperty<float>("minVelocityDampening");
	velocityDampening.max	= file.GetProperty<float>("maxVelocityDampening");
	angularVelocity.min		= file.GetProperty<float>("minAngularVelocity");
	angularVelocity.max		= file.GetProperty<float>("maxAngularVelocity");	
}

} // namespace Asteroids