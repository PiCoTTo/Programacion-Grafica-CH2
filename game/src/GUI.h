#pragma once

#include "RenderObject.h"
#include "Observer.h"
#include "ScreenGameplay.h"
#include "raylib.h"


class GUI : public RenderObject, public Observer
{
public:

	explicit GUI(ScreenGameplay* screenGameplay, Font font, Texture textureLive, Texture textureInactiveLive);

	// From RenderObject
	void render() override;

	// From Observer
	void update(Subject* subject) override;

private:
	Font m_textFont{ 0 };
	int m_livesCoordsX{ 650 };
	int m_livesCoordsY{ 10 };

	Texture m_textureLive{ 0 };
	Texture m_textureInactiveLive{ 0 };

	ScreenGameplay* m_screenGameplay;
	int m_lives{ 0 };
	int m_score{ 0 };
	int m_time{ 0 };
};