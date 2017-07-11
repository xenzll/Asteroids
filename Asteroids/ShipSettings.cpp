#include "ShipSettings.h"
#include "ConfigurationFile.hpp"

namespace Asteroids
{

void ShipSettings::LoadFromConfigurationFile(const std::string& fileName)
{
	ConfigurationFile file(fileName);

	angularDampening	= file.GetProperty<float>("angularDampening");
	velocityDampening	= file.GetProperty<float>("velocityDampening");
	approximateRadius	= file.GetProperty<float>("approximateRadius");
	rotationVelocity	= file.GetProperty<float>("rotationVelocity");
	thrustMagnitude		= file.GetProperty<float>("thrustMagnitude");
}

} // namespace Asteroids