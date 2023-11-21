#pragma once
#include "Observer.h"
#include <memory>
#include <vector>


class Observer;

class Subject
{
public:
	virtual ~Subject();

	virtual void addObserver(Observer& observer);
	virtual void removeObserver(Observer& observer);
	virtual void removeAllObservers();
	void notify();

private:
	std::vector<Observer*> m_observers;
};