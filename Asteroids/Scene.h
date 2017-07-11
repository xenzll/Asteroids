#pragma once
#include <SFML\Graphics.hpp>
#include <string>

namespace Asteroids
{

class Scene
{

public:

	Scene(sf::RenderWindow* window);
	virtual ~Scene() = default;

	virtual void Update(float deltaSeconds) = 0;
	virtual void Draw() = 0;
	virtual void OnKeyDown(sf::Keyboard::Key key) {}
	virtual void OnKeyReleased(sf::Keyboard::Key key) {}
	
protected:

	sf::RenderWindow* window;
};

} // namespace Asteroids