//
// Created by Dråfølin on 14.11.2023.
//

#include "player.h"


#include "resources.h"
#include "utilities.h"
#include "platform.h"
#include "transparentplatform.h"
#include "solidplatform.h"

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
            // Every side is relative to the platform
            QPointF side;
            int overlapLeft = right() - platform->left();
            int overlapRight = platform->right() - left();
            int overlapTop = bottom() - platform->top();
            int overlapBottom = platform->bottom() - top();

            if(overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom)
                side = QPointF(-1, 0);
            else if(overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom)
                side = QPointF(1, 0);
            else if(overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom)
                side = QPointF(0, -1);
            else if(overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop)
                side = QPointF(0, 1);

            if(dynamic_cast<SolidPlatform*>(platform))
            {
                if(side == QPointF(0, -1))
                    return false;
            } else if(dynamic_cast<TransparentPlatform*>(platform))
            {
                if(side == QPointF(0, -1) && overlapTop <= 4)
                    return false;
            }
        }
    }
    return true;
}

//! Permet de mettre a jour le joueur et d'appliquer ses physiques
//! \param deltaMs temps écoulé depuis le dernier appel de cette fonction
void Player::tick(const qreal deltaMs)
{
    qreal limit = GameFramework::meterToPx(PLAYER_SPEED) * m_playerInput.x();
    if(limit > 0)
    {
        m_velocity.setX(qMin(m_velocity.x() + GameFramework::meterToPx(PLAYER_ACCELERATION) * deltaMs / 1000, limit));
    } else
    {
        m_velocity.setX(qMax(m_velocity.x() - GameFramework::meterToPx(PLAYER_ACCELERATION) * deltaMs / 1000, limit));
    }

    for (const auto sprite : collidingSprites(sceneBoundingRect()))
    {
        if(const auto platform = dynamic_cast<Platform*>(sprite))
        {
            collideWithPlatform(platform);
        }
    }

    if(isAirborne())
    {
        m_velocity.setY(m_velocity.y() + GameFramework::GRAVITY * GameFramework::METER_PX_RATIO * deltaMs / 1000);
        if(m_jumpCharges > 0 && m_keysPressed.contains(Qt::Key_Space) && m_hasReleasedJump)
            jump();
    } else
    {
        m_jumpCharges = m_maxJumpCharges;

        if(m_keysPressed.contains(Qt::Key_Space) && m_hasReleasedJump)
            jump();
    }

    setPos(pos() + m_velocity * deltaMs / 1000);
}

//! Permet de faire sauter le joueur
void Player::jump()
{
    if(isAirborne())
       m_jumpCharges--;
    m_velocity.setY(GameFramework::METER_PX_RATIO * -6);
    m_hasReleasedJump = false;
}

//! Permet de s'occuper des touches pressées
void Player::keyPressed(const int key)
{
    m_keysPressed.append(key);
    switch (key)
    {
    case Qt::Key_A:
        m_playerInput.setX(m_playerInput.x() - 1);
        break;

    case Qt::Key_D:
        m_playerInput.setX(m_playerInput.x() + 1);
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
        m_playerInput.setX(m_playerInput.x() + 1);
        break;

    case Qt::Key_D:
        m_playerInput.setX(m_playerInput.x() - 1);
        break;

    case Qt::Key_Space:
        m_hasReleasedJump = true;
        break;

    default: break;
    }
}

void Player::collideWithPlatform(Platform* platform)
{

    auto side = platform->collisionSide(this);

    if(dynamic_cast<SolidPlatform*>(platform))
    {
        switch(side)
        {
        case GameFramework::DOWN:
            m_velocity.setY(qMax(0.0, m_velocity.y()));
            setY(platform->bottom());
            break;
        case GameFramework::LEFT:
            m_velocity.setX(qMin(0.0, m_velocity.x()));
            setX(platform->left() - width());
            break;
        case GameFramework::RIGHT:
            m_velocity.setX(qMax(0.0, m_velocity.x()));
            setX(platform->right());
            break;
        }
    }

    if(side == GameFramework::UP && (bottom() - platform->top()) <= 4)
    {
        m_velocity.setY(qMin(0.0, m_velocity.y()));
        setY(platform->top() - height() + 4);
    }
}
