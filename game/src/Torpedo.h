#pragma once
#include "RenderObject.h"


class Torpedo : public RenderObject
{
public:
	Torpedo();

	// From RenderObject
	void init() override;

	void update(float deltaTime) override;

	void render() override;

private:
	const float m_radius{ 2 };

	// Player velocity pixels/second
	const int m_maxVelocity{ 200 };
	// Rotation in degrees/second
	const int m_rotationSpeed{ 5 };
	// Fire cadence shots/second
	const int m_fireCadence{ 3 };

	float m_timeToNextShot{ 0 };

	float m_lifeTime{ 5 };
	float m_currentLife{ 0 };
};