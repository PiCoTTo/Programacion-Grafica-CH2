#include "Meteor.h"
#include "Utils.h"
#include <iostream>


Meteor::Meteor(Texture big, Texture medium, Texture small, Sound soundDownsize, Sound soundDestroy)
{
	m_type = METEOR;
	m_images.emplace(BIG, big);
	m_images.emplace(MEDIUM, medium);
	m_images.emplace(SMALL, small);
	m_soundDownSize = soundDownsize;
	m_soundDestroy = soundDestroy;
}


void Meteor::update(float deltaTime)
{
	// If the meteor is completely out of the screen, move it to the complementary position
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	if (m_triggerEnterScreen)
	{
		if ((m_position.x + m_colliderRadius) < 0 || (m_position.x - m_colliderRadius) > screenWidth ||
			(m_position.y + m_colliderRadius) < 0 || (m_position.y - m_colliderRadius) > screenHeight)
		{
			reposition();
			m_triggerEnterScreen = !m_triggerEnterScreen;
#ifdef DEBUG
			//std::cout << "Meteor repositioned\n";
#endif
		}
	}
	else
	{
		if (CheckCollisionCircleRec(m_position, static_cast<float>(m_colliderRadius), { 0,0,static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) }))
		{
			m_triggerEnterScreen = true;
#ifdef DEBUG
			//std::cout << "Meteor enters the screen\n";
#endif
		}
	}

	RenderObject::update(deltaTime);
}


void Meteor::hit(const RenderObject& renderObject)
{
	if (renderObject.m_layer == m_layer)
	{
		switch (renderObject.m_type)
		{
		case RenderObject::TORPEDO:
		{
			int size = m_currentSize;
			if (--size < SMALL)
			{
				// Destroyed
				PlaySound(m_soundDestroy);
				notify();
				m_enabled = false;
			}
			else
			{
				PlaySound(m_soundDownSize);
				notify();
				m_currentSize = static_cast<MeteorSize>(size);
				m_texture = m_images.at(m_currentSize);
				setTexture(m_texture);
			}
		} break;
		}
	}
}


void Meteor::restart()
{
	m_currentSize = BIG;
	setTexture(m_images.at(m_currentSize));
	reposition();

	m_enabled = true;
}


void Meteor::reposition()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	m_direction = { 1, 0 };
	m_direction = Vector2Rotate(m_direction, (rand() % 360) * DEG2RAD);
	m_direction = Vector2Normalize(m_direction);
	m_velocity = 20 + rand() % (m_maxVelocity - 20);
	
	Vector2 negativeDirection = { -1 * m_direction.x, -1 * m_direction.y };

	// Rotates the inverted meteor direction in a beam of 90 degrees, between -45 and 45 degrees
	Vector2 direction4Pos = Vector2Rotate(negativeDirection, (45 - (rand() % 90)) * DEG2RAD);
	// Get the meteor outside the screen boundaries along its movement line
	m_position = Vector2Scale(Vector2Normalize(direction4Pos), static_cast<float>(2 * screenWidth));
	// Adjust meteor position to the screen edges
	if (abs(m_position.x) > screenWidth / 2 + m_colliderRadius)
		m_position.x = static_cast<float>(screenWidth / 2 + m_colliderRadius) * Utils::sgn(m_position.x);
	
	if (abs(m_position.y) > screenHeight / 2 + m_colliderRadius)
		m_position.y = static_cast<float>(screenHeight / 2 + m_colliderRadius) * Utils::sgn(m_position.y);

	m_position.x += screenWidth / 2;
	m_position.y += screenHeight / 2;
	
#ifdef DEBUG
	//std::cout << "Meteor position: " << m_position.x << ", " << m_position.y << "\n";
	//std::cout << "Meteor direction: " << m_direction.x << ", " << m_direction.y << "\n";
	//std::cout << "Meteor velocity: " << m_velocity << "\n";
#endif
}
