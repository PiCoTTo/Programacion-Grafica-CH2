#pragma once

#include "Screen.h"
#include "RenderPass.h"
#include "raylib.h"
#include "Subject.h"
#include "Observer.h"
#include "Meteor.h"


class ScreenGameplay : public Screen, public Subject, public Observer
{
public:
	int getLives() { return m_lives; }
	int getScore() { return m_score; }
	float getTime() { return m_timeElapsed; }

	// From Screen
	void init() override;
	void update(float time) override;
	void render() override;
	void unload() override;

	// From Observer
	void update(Subject* subject);

private:
	int m_lives{ 3 };
	int m_score{ 0 };

	// Maximum simultaneous meteors
	int m_maxMeteors{ 3 };
	// Seconds
	float m_maxMeteorsIncreasePeriod{ 15 };
	float m_currentTimeToIncreaseMaxMeteors{ 15 };
	// Seconds between meteor spawns
	int m_meteorPeriod{ 3 };
	float m_timeForNextMeteor{ 0 };
	int m_currentMeteors{ 0 };

	// Current gameplay time
	float m_timeElapsed{ 0 };
	float m_timeSinceLastNotifiedSecond{ 0 };

	// Seconds between bonus spawns. Counts from the last pick up moment.
	int m_bonusPeriod{ 20 };
	float m_timeToNextBonus{ 20 };
	bool m_playerBonused{ false };

	Texture m_backgroundTexture{ 0 };
	Texture m_bonusTexture{ 0 };
	Sound m_soundPickUp{ 0 };
	std::vector<Texture> m_textures;
	std::vector<Sound> m_sounds;

	std::vector<Meteor*> m_subjects;

	Texture newTexture(const char* fileName)
	{
		auto texture = LoadTexture(fileName);
		m_textures.push_back(texture);
		return texture;
	}

	Sound newSound(const char* fileName)
	{
		auto sound = LoadSound(fileName);
		m_sounds.push_back(sound);
		return sound;
	}
};