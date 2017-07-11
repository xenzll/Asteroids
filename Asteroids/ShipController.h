#pragma once
#include "Ship.h"
#include "Subject.h"

#include <SFML\System.hpp>
#include <functional>

namespace Asteroids
{

class ShipController : public sf::NonCopyable, public Subject
{

public:

	ShipController();
	~ShipController() = default;

	enum Action
	{
		NoAction = -1,
		Thrust,
		RotateLeft,
		RotateRight,
		StopRotating,
		Fire,
		ActionCount
	};

	void Update();

	void AttachShip(Ship* ship);
	void DetachShip();

	void OnKeyDown(sf::Keyboard::Key key);
	void OnKeyReleased(sf::Keyboard::Key key);

	void BindKeyToAction(sf::Keyboard::Key key, Action action);
	void UnBindKey(sf::Keyboard::Key key);
	
private:

	void ClearKeyDownStates();
	void ExecuteKeyBindedFunction(sf::Keyboard::Key key);
	void ExecuteAction(Action action);
	void ClearActionToFunctionBindings();
	bool IsKeyDown(sf::Keyboard::Key key);
	bool IsKeyValid(sf::Keyboard::Key key) const;

	using ActionToFunction		= std::function<void()>[Action::ActionCount];
	using KeyToAction			= Action[sf::Keyboard::KeyCount];
	using ActionToKey			= sf::Keyboard::Key[Action::ActionCount];
	
	ActionToKey			actionToKey;
	KeyToAction			keyToAction;
	ActionToFunction	actionToFunction;
	
	// Stores the key-down status of every key
	bool keyDownStatus[sf::Keyboard::KeyCount];

	bool isShipAttached;
};

} // namespace Asteroids
