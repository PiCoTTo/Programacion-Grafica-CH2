#include "RenderObject.h"


RenderObject::RenderObject(const char* textureFile) :
	m_position({ 0, 0 }),
	m_velocity(100)
{
	setTexture(textureFile);
}


RenderObject::RenderObject(Texture texture)
{
	setTexture(texture);
}


RenderObject::RenderObject(const RenderObject& renderObject) :
	m_position(renderObject.m_position),
	m_texture(renderObject.m_texture)
{
}


void RenderObject::update(float deltaTime)
{
	// Update position according to velocity
	Vector2 deltaVel = Vector2Scale(m_direction, m_velocity * deltaTime);
	m_position = Vector2Add(m_position, deltaVel);

	m_drawDestRect = { m_position.x, m_position.y, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
}


void RenderObject::render()
{
	auto type = m_type;
	DrawTexturePro(m_texture, m_drawOriginRect, m_drawDestRect, m_drawOrigin, m_rotation, WHITE);
}


void RenderObject::setTexture(const char* textureFile)
{
	Texture texture = LoadTexture(textureFile);
	setTexture(texture);
}


void RenderObject::setTexture(Texture texture)
{
	m_texture = texture;
	// Update drawing and collision params whenever the texture changes
	m_drawOriginRect = { 0,0, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
	m_drawDestRect = { m_position.x, m_position.y, static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
	m_drawOrigin = { static_cast<float>(m_texture.width) / 2, static_cast<float>(m_texture.height) / 2 };
	m_colliderRadius = static_cast<int>(std::min(m_texture.width, m_texture.height) * 0.6f);
}
