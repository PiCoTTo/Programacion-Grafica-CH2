#pragma once

#include "RenderObject.h"
#include <map>
#include "Subject.h"


class Meteor : public RenderObject, public Subject
{
public:
	Meteor(Texture big, Texture medium, Texture small, Sound soundDownsize, Sound soundDestroy);

	typedef enum MeteorSize { SMALL, MEDIUM, BIG } MeteorSize;

	void update(float deltaTime) override;

	//void render() override;

	void hit(const RenderObject& renderObject) override;

	MeteorSize getSize() { return m_currentSize; }
	void restart();
	void reposition();

private:
	MeteorSize m_currentSize{ BIG };
	std::map<MeteorSize, Texture> m_images;
	Vector2 m_initPos{ 0.f,0.f };

	// Maximum velocity of movement
	int m_maxVelocity{ 100 };

	bool m_triggerEnterScreen{ true };

	Sound m_soundDownSize;
	Sound m_soundDestroy;
};