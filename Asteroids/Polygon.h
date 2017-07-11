#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>

namespace Asteroids
{

class ObjectRemnant;

class Polygon : public sf::Drawable, public sf::Transformable
{

public:

	virtual ~Polygon()	= default;
	
	void Update(float deltaSeconds);
	float GetApproximateRadius() const;
	bool CheckCollision(const Polygon& other);

	std::vector<std::unique_ptr<ObjectRemnant>> DecomposeIntoRemnants() const;

protected:

	Polygon() = default;

	// Returns the position of the vertex including all 
	// transformations in real world space.
	sf::Vector2f GetWorldVertexPosition(size_t vertexIndex) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::VertexArray	vertices;
	sf::Vector2f	velocity;			// meters per second
	float			velocityDampening;	// velocity decrease per second
	float			angularVelocity;	// radians per second
	float			angularDampening;	// angular velocity decrease per second
	float			approximateRadius;  // rougly the radius of the polygon
};

} // namespace Asteroids