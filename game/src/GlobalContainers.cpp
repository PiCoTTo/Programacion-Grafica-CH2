#include "GlobalContainers.h"


namespace GlobalContainers
{
	RoMap renderObjects;
	std::vector<std::weak_ptr<RenderObject>> meteorPool;
	std::vector<std::weak_ptr<Torpedo>> torpedoPool;
	Font font{ 0 };
	Sound fxCoin{ 0 };
	Music music;

	const int screenWidth = 800;
	const int screenHeight = 450;

	int hiScore;
	int score;
}
