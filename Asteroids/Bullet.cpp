#include "Bullet.h"
#include "Trigonometry.h"
#include "Random.h"
#include "SettingsCache.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Asteroids
{

Bullet::Bullet(const sf::Vector2f& position, float fireAngleDegrees)
{
	vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
	setPosition(position);
	
	// Physical properties must be intitialized before vertices
	// as vertices are dependant on them
	InitializePhysicalProperties(fireAngleDegrees);
	InitializeVertices();
}

void Bullet::InitializeVertices()
{
	// One additional vertex is required to close the shape
	vertices.resize(sideCount + 1);

	float angleInterval = (2.f * static_cast<float>(M_PI)) / static_cast<float>(sideCount);
	float currentAngle	= 0.f;

	for (size_t i = 0; i < sideCount; ++i)
	{
		vertices[i].position = sf::Vector2f(approximateRadius * cos(currentAngle), 
											approximateRadius * sin(currentAngle));

		currentAngle += angleInterval;
	}

	// The last vertex and first vertex are identical.
	// This is required to close all sides of the shape.
	vertices[sideCount].position = vertices[0].position;
}

void Bullet::InitializePhysicalProperties(float angleDegrees)
{
	const auto& Settings = SettingsCache::GetInstance().GetBulletSettings();

	approximateRadius	= Settings.approximateRadius;
	velocityMagnitude	= Settings.velocityMagnitude;
	sideCount			= Settings.sideCount;
	velocityDampening	= Settings.velocityMagnitude;
	velocityDampening	= Settings.velocityDampening;
	angularDampening	= Settings.angularDampening;
	velocity			= Trigonometry::VectorToComponents(velocityMagnitude, angleDegrees);
	angularVelocity		= Random(Settings.angularVelocity.min, Settings.angularVelocity.max);
}

} // namespace Asteroids
