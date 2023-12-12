//
// Created by Dråfølin on 12.12.2023.
//

#include "body.h"
#include "solidplatform.h"
#include "transparentplatform.h"
#include "QGraphicsScene"

//! Constructeur de la classe Body
//! \param imgPath : chemin vers l'image de l'ennemi
Body::Body(const QString& rImagePath, QGraphicsItem* pParent): Sprite(rImagePath, pParent)
{
    setOffset(-boundingRect().width() / 2, -boundingRect().height() / 2);
}

//! Permet de mettre à jour la position du corps
void Body::tick(long long elapsedTimeInMilliseconds)
{
    auto currentlyColliding = collidingSprites(sceneBoundingRect());
    for (auto collidingSprite: currentlyColliding)
    {
        if(auto collidingPlatform = dynamic_cast<Platform*>(collidingSprite))
            collideWithPlatform(collidingPlatform);
    }

    if(m_velocity.x() > MAX_SPEED_X)
        m_acceleration.setX(m_acceleration.x() + MAX_SPEED_X - m_velocity.x());
    else if(m_velocity.x() < -MAX_SPEED_X)
        m_acceleration.setX(m_acceleration.x() - MAX_SPEED_X - m_velocity.x());

    m_velocity.setX(m_velocity.x() + m_acceleration.x() * elapsedTimeInMilliseconds / 1000);
    m_velocity.setY(m_velocity.y() + m_acceleration.y() * elapsedTimeInMilliseconds / 1000);
    m_acceleration = QPointF(0, 0);

    qDebug() << m_velocity;

    auto currentPos = pos();
    currentPos.setX(pos().x() + GameFramework::meterToPx(m_velocity.x() * elapsedTimeInMilliseconds / 1000));
    currentPos.setY(pos().y() + GameFramework::meterToPx(m_velocity.y() * elapsedTimeInMilliseconds / 1000));
    setPos(currentPos);
}

//! Permet de gérer les collisions avec les plateformes
//! \param platform : pointeur vers la plateforme avec laquelle le corps est en collision
void Body::collideWithPlatform(Platform* platform)
{
    auto collidingSide = platform->collisionSide(this);
    const auto COLLISION_MARGIN = 0;
    switch(collidingSide)
    {
        case GameFramework::LEFT:
            setX(qMin(x(), platform->left() - sceneBoundingRect().width() / 2 + COLLISION_MARGIN));
            m_velocity.setX(qMin(0.0f, m_velocity.x()));
            m_acceleration.setX(qMin(0.0f, m_acceleration.x()));
            break;
        case GameFramework::RIGHT:
            setX(qMax(x(), platform->right() + sceneBoundingRect().width() / 2 - COLLISION_MARGIN));
            m_velocity.setX(qMax(0.0f, m_velocity.x()));
            m_acceleration.setX(qMax(0.0f, m_acceleration.x()));
            break;
        case GameFramework::UP:
            setY(qMin(y(), platform->top() - sceneBoundingRect().height() / 2 + COLLISION_MARGIN));
            m_velocity.setY(qMin(0.0f, m_velocity.y()));
            m_acceleration.setY(qMin(0.0f, m_acceleration.y()));
            break;
        case GameFramework::DOWN:
            setY(qMax(y(), platform->bottom() + sceneBoundingRect().height() / 2 - COLLISION_MARGIN));
            m_velocity.setY(qMax(0.0f, m_velocity.y()));
            m_acceleration.setY(qMax(0.0f, m_acceleration.y()));
            break;
        default:
            break;
    }
}


//! Permet de vérifier si l'ennemi est en l'air
//! \returns true si l'ennemi est en l'air, false sinon
bool Body::isAirborne() const
{
    for (const auto sprite : collidingSprites(sceneBoundingRect()))
    {
        if(const auto platform = dynamic_cast<Platform*>(sprite))
        {
            // Every side is relative to the platform
            QPointF side;
            const int overlapLeft = right() - platform->left();
            const int overlapRight = platform->right() - left();
            const int overlapTop = bottom() - platform->top();
            const int overlapBottom = platform->bottom() - top();

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

//! Permet de calculer et d'appliquer la gravité sur le corps
void Body::computeGravity(long long deltaMs)
{
    if(isAirborne())
        m_acceleration.setY(m_acceleration.y() + GameFramework::GRAVITY);
    else
        m_acceleration.setY(qMin(0.0f, m_acceleration.y()));
}
