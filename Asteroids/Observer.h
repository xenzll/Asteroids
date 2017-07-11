#pragma once
#include <string>

namespace Asteroids
{

class Observer
{

public:

	Observer() = default;
	virtual ~Observer() = default;

	virtual void OnNotify(const std::string& event) = 0;
};

} // namespace Asteroids