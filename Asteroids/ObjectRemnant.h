#pragma once
#include "Polygon.h"

#include <SFML\System.hpp>

namespace Asteroids
{

class ObjectRemnant : public Polygon
{

public:

	ObjectRemnant(const sf::Vector2f& pointA, const sf::Vector2f& pointB);
	~ObjectRemnant() = default;

	bool IsExpired() const;

private:

	sf::Clock	clock;
	sf::Time	lifeTime;
};

} // namespace Asteroids
