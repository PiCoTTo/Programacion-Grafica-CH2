#pragma once

#include <raylib.h>
#include <raymath.h>
#include <memory>

class RenderObject
{
public:
	enum ObjectType { UNKNOWN, METEOR, TORPEDO, PLAYER, BONUS };

	RenderObject() {};
	RenderObject(const char* texture);
	RenderObject(Texture texture);
	RenderObject(const RenderObject& renderObject);
	virtual ~RenderObject() {}

	ObjectType m_type{ UNKNOWN };
	bool m_enabled{ false };
	Vector2 m_position { 0.f, 0.f };
	Vector2 m_direction{ 0.f, 0.f };
	// Collision data
	int m_colliderRadius{ 0 };
	Rectangle m_colliderRect{ 0, 0, 0, 0 };
	int m_layer{ 0 };

	virtual void init() {};
	virtual void update(float deltaTime);
	virtual void render();
	virtual void hit(const RenderObject& renderObject) {};

	void setTexture(const char* textureFile);
	// Set the texture and update drawing and collision params
	void setTexture(Texture texture);

protected:
	// Current active texture for the object
	Texture m_texture{ 0 };
	Rectangle m_drawOriginRect{ 0 };
	Rectangle m_drawDestRect{ 0 };
	Vector2 m_drawOrigin{ 0.f,0.f };
	float m_rotation{ 0.f };
	int m_velocity{ 0 };
	float m_lastTime{ 0.0 };
};