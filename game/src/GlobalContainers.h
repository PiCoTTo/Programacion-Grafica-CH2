#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "RenderObject.h"
#include "Meteor.h"
#include "Torpedo.h"
#include "raylib.h"

#define GC GlobalContainers


namespace GlobalContainers
{
	typedef std::map<std::string, std::shared_ptr<RenderObject> > RoMap;
	extern RoMap renderObjects;
	extern std::vector<std::weak_ptr<RenderObject>> meteorPool;
	extern std::vector<std::weak_ptr<Torpedo>> torpedoPool;

	extern Font font;
	extern Sound fxCoin;
	extern Music music;

	extern const int screenWidth;
	extern const int screenHeight;

	// Gameplay
	extern int hiScore;
	extern int score;
};