#pragma once
#include "RenderObject.h"
#include "Subject.h"


class Player : public RenderObject, public Subject
{
public:
	Player(Texture texture, Sound soundShot, Sound soundShotBonused, Sound soundDestroy);
	~Player();

	void setSound(Sound soundShot);

	bool isDestroyed() { return m_destroyed; }
	bool isBonused() { return m_bonused; }

	// Relocate player on the opposite side of the screen
	void reposition();

	// From RenderObject
	void init() override;
	void update(float deltaTime) override;
	void render() override;

private:
	// Player velocity pixels/second
	const int m_maxVelocity{ 200 };
	// Rotation in degrees/second
	const int m_rotationSpeed{ 3 };
	bool m_triggerEnterScreen{ true };

	// Fire cadence shots/second
	const int m_fireCadence{ 3 };

	float m_timeToNextShot{ 0.f };

	bool m_bonused{ false };
	// seconds
	float m_maxBonusTime{ 10.f };
	float m_currentBonusTime{ 0.f };

	bool m_destroyed{ false };
	// immunity time after die
	float m_immuneTime{ 5.f };
	float m_currentImmuneTime{ 0.f };
	float m_immuneBlinkPeriod{ 5.f };

	float m_blinkTime{ 0.25f };
	float m_currentBlinkTime{ 0.f };
	bool m_showBlinked{ true };

	Sound m_soundShot;
	Sound m_soundShotBonused;
	Sound m_soundDestroy;

	void checkCollisions(float deltaTime);
};