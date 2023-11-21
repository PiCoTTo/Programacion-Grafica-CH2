#pragma once

#include "Screen.h"
#include "raylib.h"

class ScreenTitle : public Screen
{
public:
	~ScreenTitle() {}

	// From Screen
	void init() override;
	void update(float time) override;
	void render() override;
	void unload() override;

private:
	float m_blinkTime{ 2 };
	float m_currentBlinkTime{ 0 };
	bool m_showBlinked{ false };

	Texture m_texture{ 0 };
	Vector2 m_position{ 0.f, 0.f };

	Rectangle m_drawOriginRect{ 0 };
	Rectangle m_drawDestRect{ 0 };
	Vector2 m_drawOrigin{ 0.f,0.f };
	float m_rotation{ 0.f };

	float m_screenWidth;
	float m_screenHeight;

	float m_titleAnimAngle{ 0 };
	float m_titleAnimRadius{ 350 };
};