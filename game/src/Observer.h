#pragma once
#include "Subject.h"


class Subject;

class Observer
{
public:
	virtual ~Observer() {}

	virtual void update(Subject* subject) = 0;
};