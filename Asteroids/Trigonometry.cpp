#include "Trigonometry.h"

namespace Asteroids    
{
namespace Trigonometry 
{

#define _USE_MATH_DEFINES
#include <math.h>

float DegreesToRadians(float degrees)
{
	return degrees * (static_cast<float>(M_PI) / 180.f);
}

float RadiansToDegrees(float radians)
{
	return radians * (180.f / static_cast<float>(M_PI));
}

sf::Vector2f VectorToComponents(float magnitude, float degrees)
{
	float radians = DegreesToRadians(degrees);

	return sf::Vector2f(magnitude * cos(radians), magnitude * sin(radians));
}

} // namespace Trigonmetry
} // namespace Asteroids
