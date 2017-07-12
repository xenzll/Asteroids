#include "Polygon.h"
#include "ObjectRemnant.h"
#include "Trigonometry.h"

#include <math.h>

namespace Asteroids
{

void Polygon::Update(float deltaSeconds)
{
	sf::Vector2f previousPosition = getPosition();
	sf::Vector2f positionDelta	  = sf::Vector2f(velocity.x * deltaSeconds, velocity.y * deltaSeconds);

	setPosition(previousPosition + positionDelta);

	float previousRotation	= getRotation();
	float rotationDelta		= angularVelocity * deltaSeconds;

	setRotation(previousRotation + rotationDelta);

	angularVelocity *= pow(angularDampening,  deltaSeconds);
	velocity.x		*= pow(velocityDampening, deltaSeconds);
	velocity.y		*= pow(velocityDampening, deltaSeconds);
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(vertices, states);
}

float Polygon::GetApproximateRadius() const
{
	return approximateRadius;
}

bool Polygon::CheckCollision(const Polygon& other)
{
	auto positionDifference = other.getPosition() - getPosition();
	float distanceToPolygon = Trigonometry::GetVectorMagnitude(positionDifference);
	float radiiSum			= approximateRadius + other.GetApproximateRadius();
	
	// If the distance between two circle's centers 
	// is less than the sum of their radii, then
	// they must be colliding.
	if (distanceToPolygon <= radiiSum)
	{
		return true;
	}
	else
	{
		return false;
	}
}

sf::Vector2f Polygon::GetWorldVertexPosition(size_t vertexIndex) const
{
	return getTransform().transformPoint(vertices[vertexIndex].position);
}

std::vector<std::unique_ptr<ObjectRemnant>> Polygon::DecomposeIntoRemnants() const
{
	std::vector<std::unique_ptr<ObjectRemnant>> remnants;

	// Takes the polygon and breaks it into lines or remnants.
	//
	// Since there is always one additional vertex to close 
	// the shape  of the polygon visually, we must stop 1 
	// vertex early.
	for (size_t i = 0; i < vertices.getVertexCount() - 1; ++i)
	{
		auto remnant = std::make_unique<ObjectRemnant>(	GetWorldVertexPosition(i), 
														GetWorldVertexPosition(i + 1));

		remnants.push_back(std::move(remnant));
	}

	return std::move(remnants);
}

} // namespace Asteroids