#include "ScreenLogo.h"


void ScreenLogo::init()
{
	m_textureLogo = LoadTexture("resources/textures/raylib_logo.png");
	m_finishScreen = 0;
}

void ScreenLogo::update(float time)
{
	m_timeToNextScreen -= time;
	if (m_timeToNextScreen <= 0)
		m_finishScreen = 1;
}

void ScreenLogo::render()
{
	DrawTexture(m_textureLogo, 0, 0, WHITE);
}

void ScreenLogo::unload()
{
	UnloadTexture(m_textureLogo);
}
