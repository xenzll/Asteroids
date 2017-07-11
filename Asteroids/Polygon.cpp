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
	// https://stackoverflow.com/questions/8367512/
	//
	//                 This will be referred to as z
	//                  ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
	// (R0 - R1) ^ 2 <= (x0 - x1) ^ 2 + (y0 - y1) ^ 2 <= (R0 + R1) ^ 2

	float radiusDifference	= approximateRadius - other.GetApproximateRadius();
	float radiusSum			= approximateRadius + other.GetApproximateRadius();
	float xDifference		= getPosition().x - other.getPosition().x;
	float yDifference		= getPosition().y - other.getPosition().y;
	float z					= pow(xDifference, 2.f) + pow(yDifference, 2.f);

	bool colliding = (pow(radiusDifference, 2.f) <= z) && (pow(radiusSum, 2.f) >= z);

	return colliding;
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