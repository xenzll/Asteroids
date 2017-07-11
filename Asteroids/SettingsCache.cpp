#include "SettingsCache.h"

namespace Asteroids
{

BulletSettings				SettingsCache::bulletSettings;
ShipSettings				SettingsCache::shipSettings;
AsteroidGeneratorSettings	SettingsCache::asteroidGeneratorSettings;
ObjectRemnantSettings		SettingsCache::objectRemnantSettings;

void SettingsCache::LoadAllSettings()
{
	bulletSettings				.LoadFromConfigurationFile("BulletSettings");
	shipSettings				.LoadFromConfigurationFile("ShipSettings");
	asteroidGeneratorSettings	.LoadFromConfigurationFile("AsteroidGeneratorSettings");
	objectRemnantSettings		.LoadFromConfigurationFile("ObjectRemnantSettings");
}

SettingsCache& SettingsCache::GetInstance()
{
	static SettingsCache instance;
	return instance;
}

const BulletSettings& SettingsCache::GetBulletSettings() const
{
	return bulletSettings;
}

const ShipSettings&	SettingsCache::GetShipSettings() const
{
	return shipSettings;
}

const AsteroidGeneratorSettings& SettingsCache::GetAsteroidGeneratorSettings() const
{
	return asteroidGeneratorSettings;
}

const ObjectRemnantSettings& SettingsCache::GetObjectRemnantSettings() const
{
	return objectRemnantSettings;
}

} // namespace Asteroids