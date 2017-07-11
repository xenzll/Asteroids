#include "GameplayScene.h"
#include "Random.h"
#include "SoundBufferCache.h"
#include "SettingsCache.h"
#include "KeyToString.h"
#include "StringToKey.h"
#include "ConfigurationFile.hpp"

#include <algorithm>
#include <string>
#include <math.h>

namespace Asteroids
{

GameplayScene::GameplayScene(sf::RenderWindow* window)
	:
	Scene(window),
	currentAsteroidSpawnLocation(AsteroidSpawnLocation::Bottom),
	shipIsAlive(false)
{
	// Load all settings. It's crucial this happens first
	// so that other objects aren't accessing un-loaded settings.
	SettingsCache::GetInstance().LoadAllSettings();

	// Load all audio files into sound buffer cache
	SoundBufferCache::GetInstance().LoadSoundBuffers();

	// Create the ship and observe the controller for input events
	ship = std::make_unique<Ship>();
	shipController.Observe(this);

	LoadWorldSettings();
	LoadScores();
	SetSoundBuffers();
	LoadControls();
	LoadAndInitializeText();
}

void GameplayScene::LoadWorldSettings()
{
	ConfigurationFile worldSettings("WorldSettings");

	worldSize.x		= static_cast<float>(window->getSize().x);
	worldSize.y		= static_cast<float>(window->getSize().y);

	worldPadding.x	= worldSettings.GetProperty<float>("world.paddingX");
	worldPadding.y	= worldSettings.GetProperty<float>("world.paddingY");
	maxAsteroids	= worldSettings.GetProperty<size_t>("world.maxAsteroids");
	maxBullets		= worldSettings.GetProperty<size_t>("world.maxBullets");
}

void GameplayScene::LoadScores()
{
	ConfigurationFile scoreSettings("ScoreSettings");

	currentScore = 0;
	highestScore = 0;
	scorePerAsteroid[AsteroidSize::Small]	= scoreSettings.GetProperty<int>("smallAsteroid.score");
	scorePerAsteroid[AsteroidSize::Medium]	= scoreSettings.GetProperty<int>("mediumAsteroid.score");
	scorePerAsteroid[AsteroidSize::Large]	= scoreSettings.GetProperty<int>("largeAsteroid.score");
}

void GameplayScene::LoadControls()
{
	ConfigurationFile controlSettings("ControlSettings");

	using std::string;
	using Action	= ShipController::Action;

	restartGameplayKey = StringToKey(controlSettings.GetProperty<string>("playGameKey"));

	shipController.BindKeyToAction(StringToKey(controlSettings.GetProperty<string>("fireKey")),			Action::Fire);
	shipController.BindKeyToAction(StringToKey(controlSettings.GetProperty<string>("thrustKey")),		Action::Thrust);
	shipController.BindKeyToAction(StringToKey(controlSettings.GetProperty<string>("rotateLeftKey")),	Action::RotateLeft);
	shipController.BindKeyToAction(StringToKey(controlSettings.GetProperty<string>("rotateRightKey")),	Action::RotateRight);
}

void GameplayScene::LoadAndInitializeText()
{
	ConfigurationFile textSettings("TextSettings");

	auto fontFileName = textSettings.GetProperty<std::string>("fontFileName");

	if (!hyperspaceFont.loadFromFile("Resources/Fonts/" + fontFileName))
	{
		throw std::runtime_error("Failed to load font: " + fontFileName);
	}

	// Apply font on all text objects
	logo.setFont(hyperspaceFont);
	currentScoreText.setFont(hyperspaceFont);
	highestScoreText.setFont(hyperspaceFont);
	pressKeyToPlayText.setFont(hyperspaceFont);

	// Load font sizes for each text object
	logo.				setCharacterSize(textSettings.GetProperty<unsigned int>("logoSize"));
	highestScoreText.	setCharacterSize(textSettings.GetProperty<unsigned int>("highScoreSize"));
	currentScoreText.	setCharacterSize(textSettings.GetProperty<unsigned int>("scoreSize"));
	pressKeyToPlayText.	setCharacterSize(textSettings.GetProperty<unsigned int>("pressKeyToPlaySize"));

	// Set string values for each text object
	logo.setString("Asteroids");
	currentScoreText.setString("Score: 0");
	highestScoreText.setString("High Score: 0");
	pressKeyToPlayText.setString("Press " + KeyToString(restartGameplayKey) + " to play");

	// Set the origin of each text object to its' center
	// so that when displayed, everything is center-aligned
	SetTextOriginToCenter(logo);
	SetTextOriginToCenter(highestScoreText);
	SetTextOriginToCenter(currentScoreText);
	SetTextOriginToCenter(pressKeyToPlayText);

	// Place the logo roughly 1/4 from the top of the screen
	logo.setPosition(worldSize.x * 0.5f, logo.getLocalBounds().height * 2.f);

	// Place the high-score at the center of the screen
	highestScoreText.setPosition(logo.getPosition().x, worldSize.y * 0.5f);

	// Place current score at the bottom of the screen
	currentScoreText.setPosition(worldSize.x * 0.5f, worldSize.y - currentScoreText.getLocalBounds().height);

	// Place "Press [Key] to play" 1/4 from the bottom of the screen
	pressKeyToPlayText.setPosition(worldSize.x * 0.5f, worldSize.y - worldSize.y * 0.25f);

	// Load the oscillation speed to be used for the logo at the menu
	logoOscillationSpeed = textSettings.GetProperty<float>("logoOscillationSpeed");
}	

void GameplayScene::SetSoundBuffers()
{
	// The thrust sound will continue when played
	// until explicitly paused
	thrustSound.setLoop(true);

	// Takes the pre-cached SoundBuffers from the SoundBufferCache
	// and sets them to the appropriate sound.
	
	thrustSound.setBuffer(SoundBufferCache::GetInstance().GetSoundBuffer("thrust"));
	fireSound.setBuffer(SoundBufferCache::GetInstance().GetSoundBuffer("fire"));

	bangSounds[AsteroidSize::Small].setBuffer(SoundBufferCache::GetInstance().GetSoundBuffer("bangSmall"));
	bangSounds[AsteroidSize::Medium].setBuffer(SoundBufferCache::GetInstance().GetSoundBuffer("bangMedium"));
	bangSounds[AsteroidSize::Large].setBuffer(SoundBufferCache::GetInstance().GetSoundBuffer("bangLarge"));
}

void GameplayScene::DestroyAllAsteroids()
{
	for (auto& asteroid : asteroids)
	{
		BreakPolygonIntoRemnants(*asteroid);
		asteroid.reset();
	}

	asteroids.clear();
}

void GameplayScene::DestroyAllBullets()
{
	for (auto& bullet : bullets)
	{
		BreakPolygonIntoRemnants(bullet);
	}

	bullets.clear();
}

void GameplayScene::RestartGameplay()
{
	// Rotatation persists until explicitly stopped,
	// thus to prevent rotation from previous gameplay
	// from persisting, it is stopped.
	ship->StopRotating();
	
	// Re-attach control to the ship and revive it.
	shipController.AttachShip(ship.get());
	shipIsAlive = true;

	// Destroying all asteroids provides the ship
	// with a save region to spawn in as newly
	// spawned asteroids come from the sides.
	DestroyAllAsteroids();
	
	// Prevents bullets from persisting into the next 
	// game session.
	DestroyAllBullets();

	// Spawn the ship at the center of the screen.
	ship->setPosition(worldSize.x * 0.5f, worldSize.y * 0.5f);

	// Reset current score.
	currentScore = 0;
	SetScoreText("Score", currentScoreText, 0);
}

void GameplayScene::Update(float deltaSeconds)
{
	TickPhysics(deltaSeconds);
	ResolveCollisions();
	DestroyDistantObjects();
	DestroyExpiredRemnants();
	SpawnAsteroidsAsNeeded();
	shipController.Update();
}

void GameplayScene::Draw()
{
	for (auto& asteroid : asteroids)
	{
		window->draw(*asteroid);
	}

	for (auto& bullet : bullets)
	{
		window->draw(bullet);
	}

	for (auto& remnant : remnants)
	{
		window->draw(*remnant);
	}

	if (shipIsAlive)
	{
		window->draw(*ship);
		window->draw(currentScoreText);
	}
	else
	{
		window->draw(logo);
		window->draw(highestScoreText);
		window->draw(pressKeyToPlayText);
	}
}

void GameplayScene::TickPhysics(float deltaSeconds)
{
	for (auto& asteroid : asteroids)
	{
		asteroid->Update(deltaSeconds);
	}

	for (auto& remnant : remnants)
	{
		remnant->Update(deltaSeconds);
	}

	for (auto& bullet : bullets)
	{
		bullet.Update(deltaSeconds);
	}

	if (shipIsAlive)
	{
		ship->Update(deltaSeconds);
	}
	else
	{
		OscillateLogo(deltaSeconds);
	}
}

void GameplayScene::SplitAsteroid(AsteroidPtr& asteroid)
{
	AsteroidSize size		= asteroid->GetSize();
	AsteroidSize splitSize	= AsteroidSize(size - 1);

	// Too small to be split
	if (splitSize < AsteroidSize::Small)
	{
		return;
	}
	else
	{
		auto firstHalf	= asteroidGenerator.GenerateAsteroid(splitSize);
		auto secondHalf = asteroidGenerator.GenerateAsteroid(splitSize);

		firstHalf->setPosition(asteroid->getPosition());
		secondHalf->setPosition(asteroid->getPosition());
		secondHalf->move(sf::Vector2f(secondHalf->GetApproximateRadius(), firstHalf->GetApproximateRadius()));

		asteroids.push_back(std::move(firstHalf));
		asteroids.push_back(std::move(secondHalf));
	}
}

void GameplayScene::ResolveCollisions()
{
	// Bullet vs Asteroid collisions
	for (size_t b = 0; b < bullets.size(); ++b)
	{
		for (size_t a = 0; a < asteroids.size(); ++a)
		{
			// Asteroid and bullet are colliding
			if (asteroids[a]->CheckCollision(bullets[b]))
			{
				// Split the asteroid into two
				SplitAsteroid(asteroids[a]);

				// Break the split asteroid into pieces
				BreakPolygonIntoRemnants(*asteroids[a]);

				// Play appropriate bang sound
				bangSounds[asteroids[a]->GetSize()].play();

				// Add score for destroying the asteroid
				currentScore += scorePerAsteroid[asteroids[a]->GetSize()];
				SetScoreText("Score", currentScoreText, currentScore);

				// Update the highest score if need be
				highestScore = std::max(currentScore, highestScore);
				SetScoreText("High score", highestScoreText, highestScore);

				// Destroy both bullet and asteroid
				asteroids[a].reset();
				asteroids.erase(asteroids.begin() + a);
				bullets.erase(bullets.begin() + b);

				// Exit the asteroid loop because the bullet no longer exists
				break;
			}
		}
	}

	if (shipIsAlive)
	{
		// Ship vs Asteroid collision
		for (size_t a = 0; a < asteroids.size(); ++a)
		{
			if (asteroids[a]->CheckCollision(*ship))
			{
				OnShipDeath();

				// There is only one ship, thus 
				// no further checking is needed
				break;
			}
		}
	}
}

void GameplayScene::OnShipDeath()
{
	// Break the ship into pieces
	BreakPolygonIntoRemnants(*ship);

	// Remove control of the ship and kill it
	shipController.DetachShip();
	shipIsAlive = false;

	// Stop the thrusting sound to prevent it from
	// continuing playing even after the ship has died
	thrustSound.pause();
}

void GameplayScene::BreakPolygonIntoRemnants(const Polygon& polygon)
{
	auto polygonRemnants = polygon.DecomposeIntoRemnants();

	std::move(std::begin(polygonRemnants), std::end(polygonRemnants), std::back_inserter(remnants));
}

void GameplayScene::SpawnAsteroidsAsNeeded()
{
	for (size_t i = asteroids.size(); i < maxAsteroids; ++i)
	{
		SpawnAsteroidOffscreen();
	}
}

void GameplayScene::SpawnAsteroidOffscreen()
{
	// Spawns an asteroid off-screen either at one of the four sides
	// with an velocity which will carry it on-screen.
	// Each time an asteroid is spawned, the next one spawned 
	// will be placed at another one of the four sides of the screen.

	const auto& Settings = SettingsCache::GetInstance().GetAsteroidGeneratorSettings();

	auto asteroid = asteroidGenerator.GenerateAsteroid(asteroidGenerator.GenerateRandomAsteroidSize());
	auto radius   = asteroid->GetApproximateRadius();

	float xVel = Random(Settings.velocity.min, Settings.velocity.max);
	float yVel = Random(Settings.velocity.min, Settings.velocity.max);

	switch (currentAsteroidSpawnLocation)
	{
	case AsteroidSpawnLocation::Top:
		asteroid->SetVelocity(sf::Vector2f(xVel, yVel));
		asteroid->setPosition(sf::Vector2f(Random(0.f, worldSize.x), -radius));
		break;
	case AsteroidSpawnLocation::Bottom:
		asteroid->SetVelocity(sf::Vector2f(xVel, -yVel));
		asteroid->setPosition(sf::Vector2f(Random(0.f, worldSize.x), worldSize.y + radius));
		break;
	case AsteroidSpawnLocation::Left:
		asteroid->SetVelocity(sf::Vector2f(xVel, yVel));
		asteroid->setPosition(sf::Vector2f(-radius, Random(0.f, worldSize.y)));
		break;
	case AsteroidSpawnLocation::Right:
		asteroid->SetVelocity(sf::Vector2f(-xVel, yVel));
		asteroid->setPosition(sf::Vector2f(worldSize.x + radius, Random(0.f, worldSize.y)));
		break;
	}

	asteroids.push_back(std::move(asteroid));

	currentAsteroidSpawnLocation = (AsteroidSpawnLocation)((currentAsteroidSpawnLocation + 1) % AsteroidSpawnLocation::LocationCount);
}

void GameplayScene::OnNotify(const std::string& event) 
{
	if (event == "Fire" && bullets.size() < maxBullets)
	{
		fireSound.play();
		bullets.push_back(Bullet(ship->GetVertexPosition(Ship::Vertex::Nozzle), ship->getRotation()));
	}
	else if (event == "StartedThrusting")
	{
		thrustSound.play();
	}
	else if (event == "StoppedThrusting")
	{
		thrustSound.pause();
	}
}

void GameplayScene::DestroyExpiredRemnants()
{
	for (size_t i = 0; i < remnants.size(); ++i)
	{
		if (remnants[i]->IsExpired())
		{
			remnants[i].reset();
			remnants.erase(remnants.begin() + i);
		}
	}
}

void GameplayScene::DestroyDistantObjects()
{
	DestroyDistantBullets();
	DestroyDistantAsteroids();

	if (shipIsAlive && IsPolygonOffScreen(*ship))
	{
		OnShipDeath();
	}
}

bool GameplayScene::IsPolygonOutOfWorldBounds(const Polygon& polygon)
{
	float radius	= polygon.GetApproximateRadius();
	auto  position	= polygon.getPosition();

	if (
		position.x - radius	> worldSize.x + worldPadding.x	|| // Off-world right
		position.x + radius	< -worldPadding.x				|| // Off-world left
		position.y - radius	< -worldPadding.y				|| // Off-world top
		position.y + radius	> worldSize.y + worldPadding.y)	   // Off-world bottom
	{
		return true;
	}

	return false;
}

bool GameplayScene::IsPolygonOffScreen(const Polygon& polygon)
{
	float radius	= polygon.GetApproximateRadius();
	auto  position	= polygon.getPosition();

	if (
		position.x - radius	> worldSize.x	|| // Off-screen right
		position.x + radius	< 0.f			|| // Off-screen left
		position.y - radius	< 0.f			|| // Off-screen top
		position.y + radius	> worldSize.y)	   // Off-screen bottom
	{
		return true;
	}

	return false;
}

void GameplayScene::DestroyDistantBullets()
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (IsPolygonOffScreen(bullets[i]))
		{
			bullets.erase(bullets.begin() + i);
		}
	}
}

void GameplayScene::DestroyDistantAsteroids()
{
	for (size_t i = 0; i < asteroids.size(); ++i)
	{
		if (IsPolygonOutOfWorldBounds(*asteroids[i]))
		{
			asteroids[i].reset();
			asteroids.erase(asteroids.begin() + i);
		}
	}
}

void GameplayScene::OnKeyDown(sf::Keyboard::Key key)
{
	if (!shipIsAlive && key == restartGameplayKey)
	{
		RestartGameplay();
	}

	shipController.OnKeyDown(key);
}

void GameplayScene::OnKeyReleased(sf::Keyboard::Key key)
{
	shipController.OnKeyReleased(key);
}

void GameplayScene::OscillateLogo(float deltaSeconds)
{
	// Oscillates the "Asteroids" logo using a cosine wave; 
	// purley for asthetic purposes

	// Increase the current angle by the oscillation speed
	logoOscillationRadians += logoOscillationSpeed * deltaSeconds;

	// Determine the current height offset by seeing where on
	// the y-axis are on the cosine wave
	float oscillationHeightOffset = logo.getLocalBounds().height * cos(logoOscillationRadians);

	// Takes the default logo position's height, and applies
	// the height offset
	logo.setPosition(logo.getPosition().x, logo.getLocalBounds().height * 2.f + oscillationHeightOffset);
}

void GameplayScene::SetScoreText(const std::string& string, sf::Text& text, int score)
{
	text.setString(string + ": " + std::to_string(score));

	// When the score increases sufficiently enough
	// the origin of the text must be reset to accomodate
	// for the additional digits
	SetTextOriginToCenter(highestScoreText);
}

void GameplayScene::SetTextOriginToCenter(sf::Text& text)
{
	text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.5f);
}

} // namespace Asteroids