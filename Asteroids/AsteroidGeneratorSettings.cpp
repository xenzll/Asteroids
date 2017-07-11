#include "AsteroidGeneratorSettings.h"
#include "ConfigurationFile.hpp"

namespace Asteroids
{

void AsteroidGeneratorSettings::LoadFromConfigurationFile(const std::string& fileName)
{
	ConfigurationFile file(fileName);

	angularDampening.min	= file.GetProperty<float>("asteroid.minAngularDampening");
	angularVelocity.min		= file.GetProperty<float>("asteroid.minAngularVelocity");
	velocity.min			= file.GetProperty<float>("asteroid.minVelocity");
	velocityDampening.min	= file.GetProperty<float>("asteroid.minVelocityDampening");

	angularDampening.max	= file.GetProperty<float>("asteroid.maxAngularDampening");
	angularVelocity.max		= file.GetProperty<float>("asteroid.maxAngularVelocity");
	velocity.max			= file.GetProperty<float>("asteroid.maxVelocity");
	velocityDampening.max	= file.GetProperty<float>("asteroid.maxVelocityDampening");

	auto& small			= dimensions[Asteroids::AsteroidSize::Small];
	auto& medium		= dimensions[Asteroids::AsteroidSize::Medium];
	auto& large			= dimensions[Asteroids::AsteroidSize::Large];

	small.radius.min	= file.GetProperty<float>("smallAsteroid.minRadius");
	small.radius.max	= file.GetProperty<float>("smallAsteroid.maxRadius");
	small.vertexCount	= file.GetProperty<size_t>("smallAsteroid.vertexCount");

	medium.radius.min	= file.GetProperty<float>("mediumAsteroid.minRadius");
	medium.radius.max	= file.GetProperty<float>("mediumAsteroid.maxRadius");
	medium.vertexCount	= file.GetProperty<size_t>("mediumAsteroid.vertexCount");

	large.radius.min	= file.GetProperty<float>("largeAsteroid.minRadius");
	large.radius.max	= file.GetProperty<float>("largeAsteroid.maxRadius");
	large.vertexCount	= file.GetProperty<size_t>("largeAsteroid.vertexCount");
}

} // namespace Asteroids