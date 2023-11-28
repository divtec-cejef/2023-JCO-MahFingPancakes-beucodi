//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"


#include "resources.h"
#include "utilities.h"
#include "platform.h"

//! Constructeur de player
Player::Player(): Sprite(GameFramework::imagesPath() + "/Ghost GIF Frames/frame_00_delay-0.03s.gif")
{
	for (int i = 1; i < 39; i++)
	{
	    QString currentI = "0" + QString::number(i);
	    currentI = currentI.sliced(currentI.size() - 2, 2);
	    addAnimationFrame(GameFramework::imagesPath() + "/Ghost GIF Frames/frame_" + currentI + "_delay-0.03s.gif");
	}
    setAnimationSpeed(100);
    //setScale(40.0 / width());
    setDebugModeEnabled(true);
    startAnimation();
}

//! Permet de vérifier si l'ennemi est en l'air
//! \returns true si l'ennemi est en l'air, false sinon
bool Player::isAirborne() const
{
    auto colliding = collidingSprites(sceneBoundingRect());

    for (const auto sprite : colliding)
    {
        if(const auto platform = dynamic_cast<Platform*>(sprite))
        {
            if(platform->top() < bottom() - 3 &&
            platform->left() < right() + 3 &&
            platform->right() > left() - 3)
                return false;
        }
    }
    return true;
}

//! Permet de mettre a jour le joueur et d'appliquer ses physiques
//! \param deltaMs temps écoulé depuis le dernier appel de cette fonction
void Player::tick(const qreal deltaMs)
{
    setPos(pos() + m_velocity * deltaMs / 1000);

    if(isAirborne())
    {
        m_velocity.setY(m_velocity.y() + GameFramework::GRAVITY * GameFramework::METER_PX_RATIO * deltaMs / 1000);
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
        auto colliding = collidingSprites(sceneBoundingRect());
        for (const auto sprite : colliding)
        {
            if(const auto platform = dynamic_cast<Platform*>(sprite))
            {
                setY(platform->top() - height() + 4);
                break;
            }
        }

        for(const auto key: m_keysPressed)
        {
            if(key == Qt::Key_Space)
            {
                m_longJumpCooldown = 10;
                jump();
                break;
            }
        }
    }
}

//! Permet de faire sauter le joueur
void Player::jump(const bool longJump)
{
    m_velocity.setY(GameFramework::METER_PX_RATIO * (longJump ? -6 : -4));
}

//! Permet de s'occuper des touches pressées
void Player::keyPressed(const int key)
{
    m_keysPressed.append(key);
    switch (key)
    {
    case Qt::Key_A:
        m_velocity.setX(m_velocity.x() - GameFramework::meterToPx(PLAYER_SPEED));
        break;

    case Qt::Key_D:
        m_velocity.setX(m_velocity.x() + GameFramework::meterToPx(PLAYER_SPEED));
        break;

    default: break;
    }
}

//! Permet de s'occuper des touches relachées
void Player::keyReleased(const int key)
{
    m_keysPressed.removeAll(key);
    switch (key)
    {
    case Qt::Key_A:
        m_velocity.setX(m_velocity.x() + GameFramework::meterToPx(PLAYER_SPEED));
        break;

    case Qt::Key_D:
        m_velocity.setX(m_velocity.x() - GameFramework::meterToPx(PLAYER_SPEED));
        break;

    default: break;
    }
}
