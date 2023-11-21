#include "GameplayPass.h"
#include "GlobalContainers.h"


void GameplayPass::render()
{
	DrawTexture(m_backgroundTexture, 0, 0, WHITE);

	// Render every RenderObject
	for (const auto& roPair : GC::renderObjects)
	{
		auto ro = roPair.second;
		if (ro->m_enabled) ro->render();
	}
}
