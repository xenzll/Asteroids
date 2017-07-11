#include "Asteroid.h"
#include "Trigonometry.h"

#include <math.h>

namespace Asteroids
{

AsteroidSize Asteroid::GetSize() const
{
	return size;
}

void Asteroid::SetVelocity(const sf::Vector2f& velocity)
{
	this->velocity = velocity;
}

sf::Vector2f Asteroid::GetVelocity() const
{
	return velocity;
}

} // namespace Asteroids