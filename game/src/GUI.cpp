#include "GUI.h"
#include <sstream>


GUI::GUI(ScreenGameplay* screenGameplay, Font font, Texture textureLive, Texture textureInactiveLive)
{
	m_screenGameplay = screenGameplay;
	m_textFont = font;
	m_textureLive = textureLive;
	m_textureInactiveLive = textureInactiveLive;
	m_enabled = true;

	screenGameplay->addObserver(*this);
	update(screenGameplay);
}


void GUI::render()
{
	// 3 lives maximum
	for (int i = 0; i < 3; ++i)
	{
		Texture* texture = i < m_lives ? &m_textureLive : &m_textureInactiveLive;

		DrawTexture(*texture, static_cast<int>(m_position.x) + m_livesCoordsX + i * 50, static_cast<int>(m_position.y) + m_livesCoordsY, WHITE);
	}

	std::stringstream ss;
	ss << "Score:  " << m_score;
	DrawText(ss.str().c_str(), 10, 10, 20, WHITE);
	ss.str("");
	ss << "Time:  " << m_time;
	DrawText(ss.str().c_str(), 10, 40, 20, WHITE);
}


void GUI::update(Subject* subject)
{
	m_lives = m_screenGameplay->getLives();
	m_score = m_screenGameplay->getScore();
	m_time = static_cast<int>(m_screenGameplay->getTime());
}
