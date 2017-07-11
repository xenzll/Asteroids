#include "AsteroidGenerator.h"
#include "Trigonometry.h"
#include "Random.h"
#include "SettingsCache.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits.h>

namespace Asteroids
{

AsteroidGenerator::AsteroidPtr AsteroidGenerator::GenerateAsteroid(AsteroidSize size)
{
	AsteroidPtr asteroid(new Asteroid);
	asteroid->size = size;

	GenerateVertices(asteroid, size);
	GeneratePhyscicalProperties(asteroid);
	
	return std::move(asteroid);
}

AsteroidSize AsteroidGenerator::GenerateRandomAsteroidSize()
{
	return static_cast<AsteroidSize>(Random(AsteroidSize::Small, AsteroidSize::Large));
}

float AsteroidGenerator::GenerateRandomRadius(AsteroidSize size)
{
	const auto& Settings	= SettingsCache::GetInstance().GetAsteroidGeneratorSettings();
	const auto& Radius		= Settings.dimensions[size].radius;

	return Random(Radius.min, Radius.max);
}

void AsteroidGenerator::GeneratePhyscicalProperties(AsteroidPtr& asteroid)
{
	const auto& Settings	= SettingsCache::GetInstance().GetAsteroidGeneratorSettings();
	float angleDegrees		= Random(0.f, 360.f);
	float angleRadians		= Trigonometry::DegreesToRadians(angleDegrees);

	asteroid->setRotation(angleDegrees);

	float velocity		= Random(Settings.velocity.min,	 Settings.velocity.max);

	asteroid->velocity	= sf::Vector2f(velocity * cos(angleRadians), velocity * sin(angleRadians));

	asteroid->velocityDampening = Random(Settings.velocityDampening.max, Settings.velocityDampening.max);
	asteroid->angularDampening  = Random(Settings.angularDampening.max,	 Settings.angularDampening.max);
	asteroid->angularVelocity	= Random(Settings.angularVelocity.max,	 Settings.angularVelocity.max);
}

void AsteroidGenerator::GenerateVertices(AsteroidPtr& asteroid, AsteroidSize size)
{
	const auto& Settings	= SettingsCache::GetInstance().GetAsteroidGeneratorSettings();
	size_t vertexCount		= Settings.dimensions[size].vertexCount;

	// The visual vertex count is increased as one 
	// additional vertex is required to close
	// the shape of the polygon. Visually 
	// however, there will only appear to be
	// vertexCount - 1 number of sides.
	// That is to say: Requesting 3 vertices
	// will create 4, however visually there
	// will only appear to be 3 sides.

	asteroid->vertices.resize(vertexCount + 1);
	asteroid->vertices.setPrimitiveType(sf::PrimitiveType::LinesStrip);

	// Generates a random convex polygon to be used
	// as the geometry for the asteroid. Does so by
	// going clockwise from 0 to -2Pi and 
	// placing points a random radius from the 
	// center. These points are placed at equal
	// intervals throughout the circle.
	// The final point and the initial point are
	// connected to form a closed visual shape.

	// The angle interval is negative so as to move clockwise.
	float angleInterval = -(2.f *  static_cast<float>(M_PI)) / static_cast<float>(vertexCount);
	float currentAngle  = 0;

	// Store the largest radius, as it is effectivley the radius 
	// of the entire asteroid
	float largestRadius = std::numeric_limits<float>::min();

	for (size_t i = 0; i < vertexCount; ++i)
	{
		float radius	  = GenerateRandomRadius(size);
		float xCoordinate = radius * cos(currentAngle);
		float yCoordinate = radius * sin(currentAngle);

		asteroid->vertices[i].position = sf::Vector2f(xCoordinate, yCoordinate);

		currentAngle += angleInterval;

		if (radius > largestRadius)
		{
			largestRadius = radius;
		}
	}

	// Set the additional vertex to the first one to close the visual shape.
	asteroid->vertices[Settings.dimensions[size].vertexCount].position = asteroid->vertices[0].position;

	asteroid->approximateRadius = largestRadius;
}

} // namespace Asteroids
