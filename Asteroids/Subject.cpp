#include "Subject.h"

#include <algorithm>

namespace Asteroids
{

void Subject::Observe(Observer* observer)
{
	observers.push_back(observer);
}

void Subject::UnObserve(Observer* observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::NotifyObservers(const std::string& event)
{
	for (auto observer : observers)
	{
		observer->OnNotify(event);
	}
}

} // namespace Asteroids