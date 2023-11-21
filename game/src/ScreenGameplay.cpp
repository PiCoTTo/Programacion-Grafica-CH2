#include "ScreenGameplay.h"
#include "GlobalContainers.h"
#include "raylib.h"
#include "Meteor.h"
#include "Player.h"
#include "Torpedo.h"
#include "GUI.h"
#include "Bonus.h"

#include <iostream>


void ScreenGameplay::init()
{
	m_finishScreen = 0;
	m_currentMeteors = 0;
	m_timeElapsed = 0;

	// Load graphic objects
	m_backgroundTexture = newTexture("resources/textures/Landscape.png");
	m_bonusTexture = newTexture("resources/textures/Bonus.png");
	m_soundPickUp = newSound("resources/sounds/CollectBonus.wav");

	// Set up the meteorite pool
	for (int i = 1; i <= 20; ++i)
	{
		auto newMeteor = GlobalContainers::renderObjects.emplace("METEOR" + std::to_string(i), std::move(std::make_shared<Meteor>(
			newTexture("resources/textures/BigMeteor.png"),
			newTexture("resources/textures/MendiumMeteor.png"),
			newTexture("resources/textures/SmallMeteor.png"),
			newSound("resources/sounds/explosion_small.wav"),
			newSound("resources/sounds/explosion.wav")
		)));

		if (newMeteor.second)
		{
			std::dynamic_pointer_cast<Meteor>(newMeteor.first->second)->addObserver(*this);
			GlobalContainers::meteorPool.push_back(newMeteor.first->second);
		}
	}

	// Set up the torpedoes pool
	for (int i = 1; i <= 200; ++i)
	{
		auto ptrTorpedo = std::make_shared<Torpedo>();
		GC::torpedoPool.push_back(ptrTorpedo);

		auto newTorpedo = GC::renderObjects.emplace("TORPEDO" + std::to_string(i), std::move(ptrTorpedo));
	}

	// Player
	std::shared_ptr<Player> ptrPlayer = std::make_shared<Player>(
		newTexture("resources/textures/SpaceShip.png"),
		newSound("resources/sounds/shot.wav"),
		newSound("resources/sounds/ActiveBouns.wav"),
		newSound("resources/sounds/explosion.wav"));

	ptrPlayer->addObserver(*this);

	GC::renderObjects.emplace("PLAYER", std::move(ptrPlayer));

	// GUI
	std::shared_ptr<GUI> ptrGUI = std::make_shared<GUI>(this, GC::font,
		newTexture("resources/textures/Life_Active.png"),
		newTexture("resources/textures/Life_Inactive.png") );
	GC::renderObjects.emplace("GUI", std::move(ptrGUI));

	// Init every RenderObject
	for (const auto& roPair : GC::renderObjects)
	{
		auto ro = roPair.second;
		ro->init();
	}
	
	m_lives = 3;
	m_score = 0;
}


void ScreenGameplay::update(float deltaTime)
{
	m_timeElapsed += deltaTime;

	// GUI time update
	if (m_timeSinceLastNotifiedSecond >= 1)
	{
		notify();
		m_timeSinceLastNotifiedSecond = 0;
	}
	m_timeSinceLastNotifiedSecond += deltaTime;

	// Meteorite spawning
	m_timeForNextMeteor -= deltaTime;

	if (m_timeForNextMeteor <= 0 && m_currentMeteors < m_maxMeteors)
	{
		for (const auto& ptrMeteor : GC::meteorPool)
		{
			std::shared_ptr<Meteor> meteor = std::dynamic_pointer_cast<Meteor>(ptrMeteor.lock());
			if (!meteor->m_enabled)
			{
				meteor->restart();
				m_currentMeteors++;
				m_timeForNextMeteor = static_cast<float>(m_meteorPeriod);
				break;
			}
		}
	}

	// Increase dificulty when the time comes
	m_currentTimeToIncreaseMaxMeteors -= deltaTime;
	if (m_currentTimeToIncreaseMaxMeteors <= 0)
	{
		m_maxMeteors += 3;
		std::cout << "Maximum number of meteors increased to: " << m_maxMeteors << std::endl;
		m_currentTimeToIncreaseMaxMeteors = m_maxMeteorsIncreasePeriod;
	}

	// Bonus spawning
	m_timeToNextBonus -= deltaTime;

	if (m_timeToNextBonus <= 0 && (std::find_if(GC::renderObjects.begin(), GC::renderObjects.end(),
			[](const auto& e) { return e.second->m_type == RenderObject::BONUS; }) == GC::renderObjects.end()))
	{
		GC::renderObjects.emplace("BONUS", std::make_shared<Bonus>(m_bonusTexture, m_soundPickUp));
	}

	// Update every RenderObject
	for (const auto& roPair : GC::renderObjects)
	{
		auto ro = roPair.second;
		if(ro->m_enabled) ro->update(deltaTime);
	}
}


void ScreenGameplay::render()
{
	DrawTexture(m_backgroundTexture, 0, 0, WHITE);

	// Render every RenderObject
	for (const auto& roPair : GC::renderObjects)
	{
		auto ro = roPair.second;
		if (ro->m_enabled) ro->render();
	}
}


void ScreenGameplay::unload()
{
	// Delete assets
	for (auto& texture : m_textures)
		UnloadTexture(texture);
	m_textures.clear();

	for (auto& sound : m_sounds)
		UnloadSound(sound);
	m_sounds.clear();

	// Delete entities
	GC::renderObjects.clear();
	GC::meteorPool.clear();
	GC::torpedoPool.clear();

	removeAllObservers();
}


void ScreenGameplay::update(Subject* subject)
{
	Meteor* meteor = dynamic_cast<Meteor*>(subject);
	if (meteor != nullptr)
	{
		// Update score
		switch (meteor->getSize())
		{
		case Meteor::BIG:
			m_score += 10;
			break;
		case Meteor::MEDIUM:
			m_score += 20;
			break;
		case Meteor::SMALL:
			m_score += 30;
			m_currentMeteors--;
			std::cout << "Meteor destroyed!\n";
			std::cout << m_currentMeteors << "Meteors left!\n";
			break;

		default:
			break;
		}
		notify();
	}
	else
	{
		// Player stuff
		std::shared_ptr<Player> ptrPlayer = std::dynamic_pointer_cast<Player>(GC::renderObjects.at("PLAYER"));

		if (ptrPlayer != nullptr)
		{
			Player* player = ptrPlayer.get();

			if (player == subject)
			{
				if (player->isDestroyed())
				{
					std::cout << "Player destroyed!\n";
					if (--m_lives > 0)
						player->init();
					else
					{
						GC::score = m_score;
						GC::hiScore = std::max(m_score, GC::hiScore);
						m_finishScreen = 1;
					}

					notify();
				}

				if (player->isBonused() != m_playerBonused)
				{
					m_playerBonused = !m_playerBonused;
					if (m_playerBonused)
					{
						if (GC::renderObjects.erase("BONUS") <= 0)
							std::cout << "Error removing BONUS object!\n";

						m_timeToNextBonus = static_cast<float>(m_bonusPeriod);
					}
				}
			}
		}
	}
}
