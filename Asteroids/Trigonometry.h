#pragma once
#include <SFML\System.hpp>

namespace Asteroids    
{
namespace Trigonometry 
{

float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);

sf::Vector2f	VectorToComponents(float magnitude, float degrees);
float			GetVectorMagnitude(const sf::Vector2f& vector);
} // namespace Trigonmetry
} // namespace Asteroids

