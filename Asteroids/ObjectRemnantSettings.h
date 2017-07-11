#pragma once
#include <string>

namespace Asteroids
{

struct ObjectRemnantSettings
{
	void LoadFromConfigurationFile(const std::string& fileName);

	struct { float min, max; } angularDampening;
	struct { float min, max; } lifeTime;			
	struct { float min, max; } velocityX;			
	struct { float min, max; } velocityY;			
	struct { float min, max; } velocityDampening; 	
	struct { float min, max; } angularVelocity;	
};

} // namespace Asteroids