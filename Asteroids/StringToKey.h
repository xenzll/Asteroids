#pragma once
#include <SFML\Window.hpp>
#include <string>

namespace Asteroids
{

sf::Keyboard::Key StringToKey(const std::string& string);

}  // namespace Asteroids