#pragma once

#include "Screen.h"
#include <memory>
#include <map>
#include "ScreenLogo.h"
#include "ScreenTitle.h"
#include "ScreenGameplay.h"
#include "ScreenEnding.h"


class Asteroids
{
public:
	void init();
	int run();

private:

	typedef enum class ScreenId { UNKNOWN = -1, LOGO = 0, TITLE, OPTIONS, GAMEPLAY, ENDING } ScreenId;
	std::map<ScreenId, std::shared_ptr<Screen>> m_screens = {
		{ScreenId::LOGO, std::make_shared<ScreenLogo>()},
		{ScreenId::TITLE, std::make_shared<ScreenTitle>()},
		{ScreenId::GAMEPLAY, std::make_shared<ScreenGameplay>()},
		{ScreenId::ENDING, std::make_shared<ScreenEnding>()}
	};

	// Required variables to manage screen transitions (fade-in, fade-out)
	ScreenId m_currentScreenId = { ScreenId::LOGO };
	std::weak_ptr<Screen> m_currentScreen;

	ScreenId m_transToScreenId{ ScreenId::UNKNOWN };
	std::weak_ptr<Screen> m_transToScreen;
	
	float m_transAlpha = 0.0f;
	bool m_onTransition = false;
	bool m_transFadeOut = false;
	ScreenId m_transFromScreen = ScreenId::UNKNOWN;

	void changeToScreen(ScreenId screen);
	// Request transition to next screen
	void transitionToScreen(ScreenId screenId);

	void updateTransitionManagement();
	void updateTransition(void);
	void drawTransition(void);
};