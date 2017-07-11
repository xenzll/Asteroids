#include "ShipController.h"
#include <algorithm>

namespace Asteroids
{

ShipController::ShipController()
	:
	isShipAttached(false)
{
	ClearKeyDownStates();

	std::fill_n(keyToAction, static_cast<size_t>(sf::Keyboard::KeyCount),	Action::NoAction);
	std::fill_n(actionToKey, static_cast<size_t>(Action::ActionCount),		sf::Keyboard::Unknown);
}

void ShipController::AttachShip(Ship* ship)
{
	actionToFunction[Action::Thrust]		= std::bind(&Ship::Thrust,		 ship);
	actionToFunction[Action::RotateLeft]	= std::bind(&Ship::RotateLeft,	 ship);
	actionToFunction[Action::RotateRight]	= std::bind(&Ship::RotateRight,	 ship);
	actionToFunction[Action::StopRotating]	= std::bind(&Ship::StopRotating, ship);

	isShipAttached = true;
}

void ShipController::ClearKeyDownStates()
{
	std::fill_n(keyDownStatus, static_cast<size_t>(sf::Keyboard::KeyCount), false);
}

void ShipController::DetachShip()
{
	ClearActionToFunctionBindings();
	ClearKeyDownStates();
	isShipAttached = false;
}

void ShipController::OnKeyDown(sf::Keyboard::Key key)
{
	if (!isShipAttached || !IsKeyValid(key))
	{
		return;
	}

	keyDownStatus[key] = true;

	if (keyToAction[key] == Action::Thrust)
	{
		NotifyObservers("StartedThrusting");
	}
	else if (keyToAction[key] == Action::Fire)
	{
		NotifyObservers("Fire");
	}
}

void ShipController::OnKeyReleased(sf::Keyboard::Key key)
{
	if (!isShipAttached || !IsKeyValid(key))
	{
		return;
	}

	keyDownStatus[key] = false;

	// Action of the key released
	Action action = keyToAction[key];
	
	// Stop rotating when both rotation keys are released
	if (action == Action::RotateLeft)
	{
		if(!IsKeyDown(actionToKey[Action::RotateRight]))
		{
			ExecuteAction(Action::StopRotating);
		}
	}
	else if (action == Action::RotateRight)
	{
		if (!IsKeyDown(actionToKey[Action::RotateLeft]))
		{
			ExecuteAction(Action::StopRotating);
		}
	}

	// Thrust button was released
	if (action == Action::Thrust)
	{
		NotifyObservers("StoppedThrusting");
	}
}

void ShipController::Update()
{
	for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		if (keyDownStatus[i])
		{
			ExecuteKeyBindedFunction(static_cast<sf::Keyboard::Key>(i));
		}
	}
}

void ShipController::ExecuteKeyBindedFunction(sf::Keyboard::Key key)
{
	if (!IsKeyValid(key))
	{
		return;
	}

	Action action = keyToAction[key];

	// Key was not binded to an action
	if (action == Action::NoAction)
	{
		return;
	}
	
	ExecuteAction(action);
}

void ShipController::ClearActionToFunctionBindings()
{
	std::fill_n(actionToFunction, static_cast<size_t>(Action::ActionCount), nullptr);
}

void ShipController::ExecuteAction(Action action)
{
	const auto& Function = actionToFunction[action];

	// There is no ship attached, thus no functions binded
	if (!Function)
	{
		return;
	}

	Function();

	// Notify subscribers that action has been executed
	switch (action)
	{
	case Action::RotateLeft:		NotifyObservers("RotateLeft");		break;
	case Action::RotateRight:		NotifyObservers("RotateRight");		break;
	case Action::StopRotating:		NotifyObservers("StopRotating");	break;
	case Action::Thrust:			NotifyObservers("Thrust");			break;
	}
}

void ShipController::BindKeyToAction(sf::Keyboard::Key key, Action action)
{
	if (!IsKeyValid(key))
	{
		return;
	}

	keyToAction[key]	= action;
	actionToKey[action] = key;
}

void ShipController::UnBindKey(sf::Keyboard::Key key)
{
	if (!IsKeyValid(key))
	{
		return;
	}

	Action originalAction = keyToAction[key];

	actionToKey[originalAction] = sf::Keyboard::Unknown;
	keyToAction[key]			= Action::NoAction;
}

bool ShipController::IsKeyValid(sf::Keyboard::Key key) const
{
	return key != sf::Keyboard::Key::Unknown;
}

bool ShipController::IsKeyDown(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Unknown)
	{
		return false;
	}

	return keyDownStatus[key];
}

} // namespace Asteroids