#include "Ship.h"
#include "Trigonometry.h"
#include "SettingsCache.h"

namespace Asteroids
{

Ship::Ship()
{
	// Physical properties must be intitialized before vertices
	// as vertices are dependant on them
	InitializePhysicalProperties();
	InitializeVertices();
}

sf::Vector2f Ship::GetVertexPosition(Vertex vertex) const
{
	// Applies all transformations to provide the current
	// world position of the vertex
	return getTransform().transformPoint(vertices[vertex].position);
}

void Ship::InitializeVertices()
{
	// Defines the shape for an arrow to be used
	// as the geometry for the ship

	// One additional vertex is requires to close all
	// sides of the arrow
	vertices.resize(VertexCount + 1);
	
	vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
	
	/*
	
	 Left Wing
	|	\ 
	\	 \
Tail \    \   Nozzle
	 /	  /
	/	 /
	| 	/
	Right Wing

	*/

	vertices[0].position = sf::Vector2f(approximateRadius * 2.f, 0.f);		// Nozzle
	vertices[1].position = sf::Vector2f(0.f, approximateRadius * 0.5f);		// Left Wing
	vertices[2].position = sf::Vector2f(approximateRadius * 0.25f, 0.f);	// Tail
	vertices[3].position = sf::Vector2f(0.f, -approximateRadius * 0.5f);	// Right Wing
	vertices[4].position = sf::Vector2f(approximateRadius * 2.f, 0.f);		// Nozzle
}								

void Ship::InitializePhysicalProperties()
{
	const auto& Settings = SettingsCache::GetInstance().GetShipSettings();

	angularDampening	= Settings.angularDampening;
	velocityDampening	= Settings.velocityDampening;
	approximateRadius	= Settings.approximateRadius;
	rotationVelocity	= Settings.rotationVelocity;
	thrustMagnitude		= Settings.thrustMagnitude;
	velocity.x			= 0.f;
	velocity.y			= 0.f;
	angularVelocity		= 0.f;	

	setOrigin(approximateRadius, 0);
}

void Ship::Rotate(RotationDirection direction)
{
	angularVelocity = static_cast<float>(direction) * rotationVelocity;
}

void Ship::RotateLeft()
{
	Rotate(RotationDirection::Left);
}

void Ship::RotateRight()
{
	Rotate(RotationDirection::Right);
}

void Ship::StopRotating()
{
	Rotate(RotationDirection::None);
}

void Ship::Thrust()
{
	velocity = Trigonometry::VectorToComponents(thrustMagnitude, getRotation());
}

} // namespace Asteroids