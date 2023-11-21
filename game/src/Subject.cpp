#pragma once

#include "Subject.h"
#include <algorithm>


Subject::~Subject()
{
}


void Subject::addObserver(Observer& observer)
{
	m_observers.push_back(&observer);
}


void Subject::removeObserver(Observer& observer)
{
	auto o = std::find(m_observers.begin(), m_observers.end(), &observer);
	if (o != m_observers.end())
		m_observers.erase(o);
}


void Subject::removeAllObservers()
{
	m_observers.clear();
}


void Subject::notify()
{
	for (const auto& ptrObserver : m_observers)
		ptrObserver->update(this);
}
