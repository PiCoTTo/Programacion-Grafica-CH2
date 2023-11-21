#pragma once

#include "RenderObject.h"


class Bonus : public RenderObject
{
public:
	Bonus(Texture texture, Sound soundPickUp);

	void update(float deltaTime) override;

	void hit(const RenderObject& renderObject) override;

private:
	int m_maxTime{ 8 };
	int m_currentTime{ 0 };

	Sound m_soundPickUp{ 0 };
};