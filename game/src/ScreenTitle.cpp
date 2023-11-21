#include "ScreenTitle.h"
#include "GlobalContainers.h"


void ScreenTitle::init()
{
	m_finishScreen = 0;
	m_titleAnimRadius = 350;
	m_texture = LoadTexture("resources/textures/CustomTitle.png");
	m_drawOriginRect = { 0,0, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
	m_drawDestRect = { m_position.x, m_position.y, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
	m_drawOrigin = { static_cast<float>(m_texture.width) / 2, static_cast<float>(m_texture.height) / 2 };

	m_screenWidth = static_cast<float>(GetScreenWidth());
	m_screenHeight = static_cast<float>(GetScreenHeight());
}


void ScreenTitle::update(float deltaTime)
{
	if (IsKeyPressed(KEY_ENTER))
	{
		//finishScreen = 1;   // OPTIONS
		m_finishScreen = 2;   // GAMEPLAY
		PlaySound(GC::fxCoin);
	}

	m_currentBlinkTime += deltaTime;
	if (m_currentBlinkTime >= m_blinkTime)
	{
		m_showBlinked = !m_showBlinked;
		m_currentBlinkTime = 0;
	}

	if (m_titleAnimRadius >= 0)
	{
		m_titleAnimAngle++;
		m_titleAnimRadius--;
		m_position.x = m_titleAnimRadius * cosf(m_titleAnimAngle * DEG2RAD);
		m_position.y = m_titleAnimRadius * sinf(m_titleAnimAngle * DEG2RAD);
		m_drawDestRect = { m_screenWidth / 2 + m_position.x, m_screenHeight / 2 + m_position.y, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
	}
}


void ScreenTitle::render()
{
	DrawTexturePro(m_texture, m_drawOriginRect, m_drawDestRect, m_drawOrigin, m_rotation, WHITE);
	
	if(m_showBlinked)
		DrawTextEx(GC::font, "PRESS ENTER", {m_screenWidth/2 - 130, m_screenHeight/2 + 100}, GC::font.baseSize * 3.0f, 4, DARKBLUE);
}


void ScreenTitle::unload()
{
	UnloadTexture(m_texture);
}
