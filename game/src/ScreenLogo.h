#pragma once

#include "Screen.h"
#include "raylib.h"

class ScreenLogo : public Screen
{
public:
	~ScreenLogo() {}

	// From Screen
	void init() override;
	void update(float time) override;
	void render() override;
	void unload() override;

private:
	Texture m_textureLogo{ 0 };

	float m_timeToNextScreen{ 5 };
};