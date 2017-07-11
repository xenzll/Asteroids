#pragma once
#include "Polygon.h"

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>

namespace Asteroids
{

class Ship : public Polygon
{

public:

	Ship();
	~Ship() = default;

	enum Vertex
	{
		// Order must not change

		Nozzle,
		LeftWing,
		Tail,
		RightWing,
		VertexCount
	};

	sf::Vector2f GetVertexPosition(Vertex vertex) const;

	void Thrust();
	void RotateLeft();
	void RotateRight();
	void StopRotating();
	
private:
	
	enum RotationDirection
	{
		Left	= -1,
		None	= 0,
		Right	= 1		
	};

	void Rotate(RotationDirection direction);
	void InitializeVertices();
	void InitializePhysicalProperties();

	// Amount to thrust in direction
	float thrustMagnitude;

	// Amount of angular velocity to apply on rotation
	float rotationVelocity;
};

} // namespace Asteroids