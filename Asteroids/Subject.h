#pragma once
#include <string>
#include <vector>

#include "Observer.h"

namespace Asteroids
{

class Subject
{

public:

	Subject() = default;
	virtual ~Subject() = default;

	void Observe(Observer* observer);
	void UnObserve(Observer* observer);

protected:

	void NotifyObservers(const std::string& event);

private:

	std::vector<Observer*> observers;
};

} // namespace Asteroids