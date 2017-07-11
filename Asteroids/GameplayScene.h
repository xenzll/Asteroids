#pragma once
#include "Scene.h"
#include "Asteroid.h"
#include "AsteroidGenerator.h"
#include "Ship.h"
#include "ShipController.h"
#include "Bullet.h"
#include "Observer.h"
#include "ObjectRemnant.h"

#include <SFML\Audio.hpp>
#include <string>
#include <vector>
#include <memory>

namespace Asteroids
{

class GameplayScene : public Scene, public Observer
{

public:

	GameplayScene(sf::RenderWindow* window);
	~GameplayScene() = default;

	void OnKeyDown(sf::Keyboard::Key key) override;
	void OnKeyReleased(sf::Keyboard::Key key) override;
	void Update(float deltaSeconds) override;
	void Draw() override;
	void OnNotify(const std::string& event) override;

private:

	enum AsteroidSpawnLocation
	{
		Top,
		Bottom,
		Left,
		Right,
		LocationCount
	};

	using RemnantPtr			= std::unique_ptr<ObjectRemnant>;
	using AsteroidPtr			= std::unique_ptr<Asteroid>;
	using ShipPtr				= std::unique_ptr<Ship>;

	void LoadAndInitializeText();
	void LoadWorldSettings();
	void LoadControls();
	void LoadScores();
	void SetSoundBuffers();

	void RestartGameplay();
	void OscillateLogo(float deltaSeconds);
	void SetScoreText(const std::string& string, sf::Text& text, int score);
	void TickPhysics(float deltaSeconds);
	void ResolveCollisions();
	
	void DestroyDistantObjects();
	void DestroyDistantBullets();
	void DestroyDistantAsteroids();
	void DestroyExpiredRemnants();
	void DestroyAllAsteroids();
	void DestroyAllBullets();
	void OnShipDeath();

	void SpawnAsteroidOffscreen();
	void SpawnAsteroidsAsNeeded();

	void SplitAsteroid(AsteroidPtr& asteroid);
	void BreakPolygonIntoRemnants(const Polygon& polygon);
	bool IsPolygonOutOfWorldBounds(const Polygon& polygon);
	bool IsPolygonOffScreen(const Polygon& polygon);

	void SetTextOriginToCenter(sf::Text& text);

	AsteroidSpawnLocation		currentAsteroidSpawnLocation;
	size_t						maxAsteroids;
	size_t						maxBullets;
	ShipController				shipController;
	ShipPtr						ship;
	std::vector<RemnantPtr>		remnants;
	std::vector<Bullet>			bullets;
	std::vector<AsteroidPtr>	asteroids;
	AsteroidGenerator			asteroidGenerator;
	
	sf::Keyboard::Key restartGameplayKey;

	sf::Sound thrustSound;
	sf::Sound fireSound;
	sf::Sound bangSounds[AsteroidSize::Count];

	sf::Font hyperspaceFont;
	sf::Text logo;
	sf::Text currentScoreText;
	sf::Text highestScoreText;
	sf::Text pressKeyToPlayText;

	float logoOscillationSpeed;
	float logoOscillationRadians;

	int currentScore;
	int highestScore;
	int scorePerAsteroid[AsteroidSize::Count];

	// Size of the visible world
	sf::Vector2f worldSize;	

	// Amount of padding that extends beyond the 
	// visible in both negative and positive directions
	sf::Vector2f worldPadding;

	bool shipIsAlive;
};

} // namespace Asteroids