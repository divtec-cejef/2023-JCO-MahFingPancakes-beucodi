//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"

#include "globals.h"

#include "resources.h"
#include "utilities.h"
#include "platform.h"

//! Constructeur de player
Player::Player(): Sprite(GameFramework::imagesPath() + "/player.png")
{
    setDebugModeEnabled(true);
}

//! Permet de vérifier si l'ennemi est en l'air
//! \returns true si l'ennemi est en l'air, false sinon
bool Player::isAirborne() const
{
    auto colliding = collidingSprites();

    for (auto sprite : colliding)
    {
        if(auto platform = dynamic_cast<Platform*>(sprite))
        {
            return false;
        }
    }
    return true;
}

//! Permet de mettre a jour le joueur et d'appliquer ses physiques
//! \param deltaMs temps écoulé depuis le dernier appel de cette fonction
void Player::tick(qreal deltaMs)
{
    setPos(pos() + m_velocity * deltaMs / 1000);

    if(isAirborne())
    {
        m_velocity.setY(m_velocity.y() + Globals::GRAVITY * Globals::METER_PX_RATIO * deltaMs / 1000);
        if(0 < m_longJumpCooldown)
        {
            if(m_longJumpCooldown <= 1 && m_keysPressed.contains(Qt::Key_Space))
            {
                jump(true);
                m_longJumpCooldown = 0;
            }

            m_longJumpCooldown--;
        }
    } else
    {
        m_velocity.setY(0);
        auto colliding = collidingSprites();
        for (auto sprite : colliding)
        {
            if(auto platform = dynamic_cast<Platform*>(sprite))
            {
                setY(platform->top() - height() + 3);
                break;
            }
        }

        if(m_keysPressed.contains(Qt::Key_Space))
        {
            jump();
            m_longJumpCooldown = 4;
        }
    }
}

//! Permet de faire sauter le joueur
void Player::jump(bool longJump)
{
    m_velocity.setY(Globals::METER_PX_RATIO * (longJump ? -6 : -4));
}

//! Permet de s'occuper des touches pressées
void Player::keyPressed(int key)
{
m_keysPressed.append(key);
}

//! Permet de s'occuper des touches relachées
void Player::keyReleased(int key)
{
    m_keysPressed.removeAll(key);
}
