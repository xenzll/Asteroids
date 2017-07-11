#include "Game.h"
#include "GameplayScene.h"
#include "ConfigurationFile.hpp"
#include "StringToKey.h"

#include <exception>
#include <string>

namespace Asteroids
{

Game::Game()
	:
	accumulatedTime(sf::Time::Zero)
{
	LoadGraphicsSettings();
	CreateWindow();
	LoadAndSetIcon();
	LoadExitGameKey();

	currentScene = std::make_unique<GameplayScene>(&window);
}

void Game::LoadExitGameKey()
{
	ConfigurationFile controls("ControlSettings");

	exitGameKey = StringToKey(controls.GetProperty<std::string>("exitGameKey"));
}

void Game::CreateWindow()
{
	if (fullScreen)
	{
		window.create(sf::VideoMode(windowDimensions.x, windowDimensions.y), "Asteroids", sf::Style::Fullscreen);
	}
	else
	{
		window.create(sf::VideoMode(windowDimensions.x, windowDimensions.y), "Asteroids", sf::Style::Close);
	}

	window.setKeyRepeatEnabled(false);
}

void Game::LoadGraphicsSettings()
{
	ConfigurationFile graphicsSettings("GraphicsSettings");

	windowDimensions.x	= graphicsSettings.GetProperty<unsigned int>("windowWidth");
	windowDimensions.y	= graphicsSettings.GetProperty<unsigned int>("windowHeight");
	fullScreen			= graphicsSettings.GetProperty<bool>("fullScreen");
	size_t fps			= graphicsSettings.GetProperty<size_t>("framesPerSecond");
	timePerTick			= sf::seconds(1.f / static_cast<float>(fps));
}

void Game::LoadAndSetIcon()
{
	if (!windowIcon.loadFromFile("Resources/Icons/icon.png"))
	{
		throw std::runtime_error("Failed to load icon");
	}

	window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());
}

void Game::Run()
{
	clock.restart();

	while (window.isOpen())
	{
		accumulatedTime += clock.restart();

		if (accumulatedTime < timePerTick)
		{
			sf::sleep(timePerTick - accumulatedTime);
		}

		while (accumulatedTime >= timePerTick)
		{
			accumulatedTime -= timePerTick;

			HandleEvents();
			currentScene->Update(timePerTick.asSeconds());
		}

		window.clear();
		currentScene->Draw();
		window.display();
	}
}

void Game::HandleEvents()
{
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:

			if (event.key.code == exitGameKey)
			{
				window.close();
			}

			currentScene->OnKeyDown(event.key.code);

			break;
		case sf::Event::KeyReleased:
			currentScene->OnKeyReleased(event.key.code);
			break;
		}
	}
}

} // namespace Asteroids

