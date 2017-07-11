#pragma once
#include "Ship.h"
#include "Polygon.h"
#include "Bullet.h"
#include "AsteroidGeneratorSettings.h"

#include <SFML\Graphics.hpp>
#include <vector>

namespace Asteroids
{

class Asteroid : public Polygon
{

	friend class AsteroidGenerator;

public:

	~Asteroid() = default;

	void			SetVelocity(const sf::Vector2f& velocity);
	sf::Vector2f	GetVelocity() const;
	AsteroidSize	GetSize() const;
	
private:

	// Only allow AsteroidGenerator to create asteroids
	Asteroid() = default;

	AsteroidSize size;
};

} // namespace Asteroids