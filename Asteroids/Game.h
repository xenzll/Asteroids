#pragma once
#include "Scene.h"

#include <SFML\Graphics.hpp>
#include <memory>

namespace Asteroids
{

class Game
{

public:

	Game();
	~Game() = default;

	void Run();

private:

	void HandleEvents();
	void LoadAndSetIcon();
	void LoadGraphicsSettings();
	void CreateWindow();
	void LoadExitGameKey();

	sf::RenderWindow		window;
	sf::Clock				clock;
	sf::Event				event;
	sf::Time				accumulatedTime;
	sf::Image				windowIcon;
	sf::Keyboard::Key		exitGameKey;
	sf::Time				timePerTick;
	sf::Vector2u			windowDimensions;
	std::unique_ptr<Scene>	currentScene;
	bool					fullScreen;
};

} // namespace Asteroids