#pragma once
#include <string>
#include "BulletSettings.h"
#include "ShipSettings.h"
#include "AsteroidGeneratorSettings.h"
#include "ObjectRemnantSettings.h"

/*
Responsible for loading and holding onto game settings used.
Allows game objects to access the settings from anywhere without
having to pass settings around, or load the settings themselves.
Furthermore prevents loading settings more than once.
*/

namespace Asteroids
{

class SettingsCache
{

public:

	~SettingsCache() = default;

	static void LoadAllSettings();

	static SettingsCache& GetInstance();

	const BulletSettings&			 GetBulletSettings() const;
	const ShipSettings&				 GetShipSettings() const;
	const AsteroidGeneratorSettings& GetAsteroidGeneratorSettings() const;
	const ObjectRemnantSettings&	 GetObjectRemnantSettings() const;

private:

	static BulletSettings				bulletSettings;
	static ShipSettings					shipSettings;
	static AsteroidGeneratorSettings	asteroidGeneratorSettings;
	static ObjectRemnantSettings		objectRemnantSettings;
};

}