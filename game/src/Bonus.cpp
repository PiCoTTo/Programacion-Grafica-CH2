#include "Bonus.h"


Bonus::Bonus(Texture texture, Sound soundPickUp) : RenderObject(texture)
{
	m_type = BONUS;
	m_soundPickUp = soundPickUp;
	m_enabled = true;
	m_position.x = m_texture.width / 2 + static_cast<float>(rand() % (GetScreenWidth() - m_texture.width));
	m_position.y = m_texture.height / 2 + static_cast<float>(rand() % (GetScreenHeight() - m_texture.height));
	setTexture(texture);
}


void Bonus::update(float deltaTime)
{

}

void Bonus::hit(const RenderObject& renderObject)
{
	PlaySound(m_soundPickUp);
	m_enabled = false;
}
