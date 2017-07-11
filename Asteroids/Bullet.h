#pragma once
#include "Polygon.h"
#include "BulletSettings.h"

namespace Asteroids
{

class Bullet : public Polygon
{

public:

	Bullet(const sf::Vector2f& position, float fireAngleDegrees);
	~Bullet() = default;

private:

	void InitializeVertices();
	void InitializePhysicalProperties(float angleDegrees);

	size_t sideCount;
	float  velocityMagnitude;
};

} // namespace Asteroids