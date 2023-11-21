#include "ScreenEnding.h"
#include "GlobalContainers.h"
#include <sstream>


void ScreenEnding::init()
{
	m_finishScreen = 0;
}


void ScreenEnding::update(float time)
{
	if (IsKeyPressed(KEY_ENTER))
	{
		m_finishScreen = 1;
		PlaySound(GC::fxCoin);
	}
}


void ScreenEnding::render()
{
	// Render the text
	Vector2 pos = { 300, 100 };
	DrawTextEx(GC::font, "GAME OVER", pos, GC::font.baseSize * 3.0f, 4, DARKBLUE);
	
	std::stringstream ss;
	ss << "Current Score   " << GC::score;
	pos.x = 200;
	pos.y += 140;
	DrawTextEx(GC::font, ss.str().c_str(), pos, GC::font.baseSize * 3.0f, 4, DARKBLUE);
	ss.str("");
	ss << "High Score   " << GC::hiScore;
	pos.y += 40;
	DrawTextEx(GC::font, ss.str().c_str(), pos, GC::font.baseSize * 3.0f, 4, DARKBLUE);
}


void ScreenEnding::unload()
{
	UnloadTexture(m_textureLogo);
}
