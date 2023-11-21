#include "Torpedo.h"
#include "GlobalContainers.h"


Torpedo::Torpedo()
{
    m_type = TORPEDO;
    m_velocity = 250;
}


void Torpedo::init()
{
    m_currentLife = m_lifeTime;
}


void Torpedo::update(float deltaTime)
{
    // Check collision with meteors
    for (const auto& ptrMeteor : GC::meteorPool)
    {
        auto meteor = ptrMeteor.lock();

        if (meteor->m_enabled)
        {
            if (CheckCollisionCircles(m_position, m_radius, meteor->m_position, static_cast<float>(meteor->m_colliderRadius)))
            {
                meteor->hit(*this);
                m_enabled = false;
                break;
            }
        }
    }

    RenderObject::update(deltaTime);

    // If life time reaches the end free the torpedo in the pool
    m_currentLife -= deltaTime;
    if (m_currentLife <= 0)
        m_enabled = false;
}


void Torpedo::render()
{
    DrawCircle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_radius, WHITE);
}
