#include "Player.h"
#include "GlobalContainers.h"
#include "Torpedo.h"
#include <iostream>
#include "Utils.h"


Player::Player(Texture texture, Sound soundShot, Sound soundShotBonused, Sound soundDestroy)
{
    m_type = PLAYER;
    setTexture(texture);
    m_soundShot = soundShot;
    m_soundDestroy = soundDestroy;
    m_soundShotBonused = soundShotBonused;
}


Player::~Player()
{
    removeAllObservers();
}


void Player::setSound(Sound soundShot)
{
    m_soundShot = soundShot;
}


void Player::reposition()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // TODO Casuistry needs to be revised. Works 
    if (m_position.x < -m_colliderRadius || m_position.x > screenWidth + m_colliderRadius)
        m_position.x = (abs(m_position.x - static_cast<float>(screenWidth / 2)) - static_cast<float>(screenWidth * 2 - 150)) * Utils::sgn(m_position.x);

    if (m_position.y < -m_colliderRadius || m_position.y > screenHeight + m_colliderRadius)
        m_position.y = (abs(m_position.y - static_cast<float>(screenHeight / 2)) - static_cast<float>(screenHeight * 2 - 150)) * Utils::sgn(m_position.y);
}


void Player::checkCollisions(float deltaTime)
{
    if (m_currentImmuneTime > 0)
        return;

    // Check collision with meteors
    for (const auto& ptrMeteor : GC::meteorPool)
    {
        auto meteor = ptrMeteor.lock();

        if (meteor->m_enabled)
        {
            if (CheckCollisionCircles(m_position, static_cast<float>(m_colliderRadius), meteor->m_position, static_cast<float>(meteor->m_colliderRadius)))
            {
                PlaySound(m_soundDestroy);
                m_destroyed = true;
                notify();
                m_currentImmuneTime = m_immuneTime;

                break;
            }
        }
    }

    // Check collision with bonus
    if (m_bonused)
    {
        m_currentBonusTime -= deltaTime;
        if (m_currentBonusTime <= 0)
        {
            m_bonused = false;
            notify();
        }
    }
    else
    {
        auto it = GC::renderObjects.find("BONUS");
        if (it != GC::renderObjects.end())
        {
            auto ptrBonus = it->second;
            if (ptrBonus->m_enabled)
            {
                if (CheckCollisionCircles(m_position, static_cast<float>(m_colliderRadius), ptrBonus->m_position, static_cast<float>(ptrBonus->m_colliderRadius)))
                {
                    ptrBonus->hit(*this);
                    m_currentBonusTime = m_maxBonusTime;
                    m_bonused = true;
                    notify();
                }
            }
        }
    }
}


void Player::init()
{
    m_position.x = static_cast<float>(GetScreenWidth() / 2);
    m_position.y = static_cast<float>(GetScreenHeight() / 2);
    m_rotation = 0;
    m_destroyed = false;
    m_enabled = true;
}


void Player::update(float deltaTime)
{
    // Movement
    m_velocity = 0;
    m_direction = Vector2Zero();

    if (IsKeyDown(KEY_LEFT))
        m_rotation -= m_rotationSpeed;

    if (IsKeyDown(KEY_RIGHT))
        m_rotation += m_rotationSpeed;

    if (IsKeyDown(KEY_UP))
        m_velocity = m_maxVelocity;

    m_direction = Vector2Rotate({ 0, -1 }, m_rotation * DEG2RAD);
    m_direction = Vector2Normalize(m_direction);



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
            //std::cout << "Player repositioned\n";
#endif
        }
    }
    else
    {
        if (CheckCollisionCircleRec(m_position, static_cast<float>(m_colliderRadius), { 0,0,static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) }))
        {
            m_triggerEnterScreen = true;
#ifdef DEBUG
            //std::cout << "Player enters the screen\n";
#endif
        }
    }



    if (m_timeToNextShot > 0) m_timeToNextShot -= deltaTime;

    if (IsKeyPressed(KEY_SPACE) && m_timeToNextShot <= 0)
    {
        // Fire
        if (!m_bonused)
        {
            for (const auto& ptrTorpedo : GC::torpedoPool)
            {
                auto torpedo = ptrTorpedo.lock();
                if (!torpedo->m_enabled)
                {
                    PlaySound(m_soundShot);
                    torpedo->m_position = Vector2Add(m_position, Vector2Scale(m_direction, static_cast<float>(m_colliderRadius)));
                    torpedo->m_direction = m_direction;
                    torpedo->m_enabled = true;
                    torpedo->init();
                    m_timeToNextShot = 1 / static_cast<float>(m_fireCadence);
                    break;
                }
            }
        }
        else
        {
            // Bonused fire
            int numTorpedoes = 3;
            // beam of 30 degrees
            float angles[] = { -15.f, 0.f, 15.f };
            for (const auto& ptrTorpedo : GC::torpedoPool)
            {
                auto torpedo = ptrTorpedo.lock();
                if (!torpedo->m_enabled)
                {
                    PlaySound(m_soundShotBonused);
                    torpedo->m_position = Vector2Add(m_position, Vector2Scale(m_direction, static_cast<float>(m_colliderRadius)));

                    torpedo->m_direction = m_direction;
                    torpedo->m_direction = Vector2Rotate(torpedo->m_direction, angles[numTorpedoes - 1] * DEG2RAD);
                    torpedo->m_enabled = true;
                    torpedo->init();

                    if (--numTorpedoes > 0)
                    {
                        m_timeToNextShot = 1 / static_cast<float>(m_fireCadence);
                    }
                    else
                        break;
                }
            }
        }
    }

    checkCollisions(deltaTime);

    if (m_currentImmuneTime > 0)
    {
        m_currentImmuneTime -= deltaTime;

        m_currentBlinkTime += deltaTime;
        if (m_currentBlinkTime >= m_blinkTime)
        {
            m_showBlinked = !m_showBlinked;
            m_currentBlinkTime = 0;
        }
    }

    RenderObject::update(deltaTime);
}

void Player::render()
{
    if (m_showBlinked)
        RenderObject::render();
}
