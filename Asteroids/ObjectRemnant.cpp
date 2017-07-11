#include "ObjectRemnant.h"
#include "Random.h"
#include "Trigonometry.h"
#include "SettingsCache.h"

#include <math.h>

namespace Asteroids
{

ObjectRemnant::ObjectRemnant(const sf::Vector2f& pointA, const sf::Vector2f& pointB)
{
	clock.restart();

	vertices.setPrimitiveType(sf::PrimitiveType::Lines);

	vertices.resize(2);

	// Rotates the line so as to lay on the x-axis.
	// This allows SFML to correctly place the origin
	// at the center of the line when the two vertices
	// are specified.
	//
	// After the line is constructed, it is rotated back
	// into its original position

	float width			= pointB.x - pointA.x;
	float height		= pointB.y - pointA.y;
	float angleDegrees	= Trigonometry::RadiansToDegrees(atan2f(height, width));
	float lineLength	= sqrt(pow(pointB.x - pointA.x, 2.f) + pow(pointB.y - pointA.y, 2.f));

	vertices[0].position = sf::Vector2f(0.f, 0.f);
	vertices[1].position = sf::Vector2f(width, 0.f);
	
	setRotation(angleDegrees);
	setPosition(pointA);
	
	const auto& Settings = SettingsCache::GetInstance().GetObjectRemnantSettings();

	angularDampening	= Random(Settings.angularDampening.min,			Settings.angularDampening.max);
	angularVelocity		= Random(Settings.angularVelocity.min,			Settings.angularVelocity.max);
	velocityDampening	= Random(Settings.velocityDampening.min,		Settings.velocityDampening.max);

	lifeTime			= sf::seconds(Random(Settings.lifeTime.min,		Settings.lifeTime.max));

	velocity			= sf::Vector2f(Random(Settings.velocityX.min,	Settings.velocityX.min), 
									   Random(Settings.velocityY.max,	Settings.velocityY.max));
}

bool ObjectRemnant::IsExpired() const
{
	return clock.getElapsedTime() > lifeTime;
}

} // namespace Asteroids
