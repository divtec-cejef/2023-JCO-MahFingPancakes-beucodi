//
// Created by Dråfølin on 12.12.2023.
//

#include "body.h"

#include "fragileplatform.h"
#include "solidplatform.h"
#include "transparentplatform.h"
#include "QGraphicsScene"

//! Constructeur de la classe Body
//! \param rImagePath : chemin vers l'image du corps
//! \param pParent : pointeur vers l'objet parent
Body::Body(const QString& rImagePath, QGraphicsItem* pParent): Sprite(rImagePath, pParent)
{
}

//! Permet de mettre à jour la position du corps
//! \param elapsedTimeInMilliseconds : temps écoulé depuis le dernier appel de cette fonction
void Body::tick(const long long elapsedTimeInMilliseconds)
{
    const auto deltaMs = static_cast<qreal>(elapsedTimeInMilliseconds);
    for (const auto collidingSprite : collidingSprites(sceneBoundingRect()))
    {
        if (const auto collidingPlatform = dynamic_cast<Platform*>(collidingSprite))
            collideWithPlatform(collidingPlatform);
    }

    if (m_velocity.x() > MAX_SPEED_X)
        m_acceleration.setX(m_acceleration.x() + MAX_SPEED_X - m_velocity.x());
    else if (m_velocity.x() < -MAX_SPEED_X)
        m_acceleration.setX(m_acceleration.x() - MAX_SPEED_X - m_velocity.x());

    m_velocity.setX(m_velocity.x() + m_acceleration.x() * deltaMs / 1000);
    m_velocity.setY(m_velocity.y() + m_acceleration.y() * deltaMs / 1000);
    m_acceleration = QPointF(0, 0);

    auto currentPos = pos();
    currentPos.setX(pos().x() + GameFramework::meterToPx(m_velocity.x() * deltaMs / 1000));
    currentPos.setY(pos().y() + GameFramework::meterToPx(m_velocity.y() * deltaMs / 1000));
    setPos(currentPos);
}

//! Permet de gérer les collisions avec les plateformes
//! \param platform : pointeur vers la plateforme avec laquelle le corps est en collision
void Body::collideWithPlatform(Platform* platform)
{
    switch (platform->collisionSide(this))
    {
    case GameFramework::LEFT:
        setX(qMin(x(), platform->left() - sceneBoundingRect().width()) + 1);
        m_velocity.setX(qMin(0.0f, m_velocity.x()));
        m_acceleration.setX(qMin(0.0f, m_acceleration.x()));
        break;
    case GameFramework::RIGHT:
        setX(qMax(x(), platform->right() + sceneBoundingRect().width()) - 1);
        m_velocity.setX(qMax(0.0f, m_velocity.x()));
        m_acceleration.setX(qMax(0.0f, m_acceleration.x()));
        break;
    case GameFramework::UP:
        setY(qMin(y(), platform->top() - sceneBoundingRect().height()) + 1);
        m_velocity.setY(qMin(0.0f, m_velocity.y()));
        m_acceleration.setY(qMin(0.0f, m_acceleration.y()));
        break;
    case GameFramework::DOWN:
        setY(qMax(y(), platform->bottom() + sceneBoundingRect().height()) - 1);
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
        if (const auto platform = dynamic_cast<Platform*>(sprite))
        {
            // Every side is relative to the platform
            QPointF side;
            const int overlapLeft = right() - platform->left();
            const int overlapRight = platform->right() - left();
            const int overlapTop = bottom() - platform->top();
            const int overlapBottom = platform->bottom() - top();

            if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom)
                side = QPointF(-1, 0);
            else if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom)
                side = QPointF(1, 0);
            else if (overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom)
                side = QPointF(0, -1);
            else if (overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop)
                side = QPointF(0, 1);

            if (dynamic_cast<TransparentPlatform*>(platform))
            {
                if (side == QPointF(0, -1) &&
                    overlapTop < 4 &&
                    m_velocity.y() >= 0)
                    return false;
                continue;
            }
            if (dynamic_cast<FragilePlatform*>(platform))
                continue;

            if (side == QPointF(0, -1))
                return false;
        }
    }
    return true;
}

//! Permet de calculer et d'appliquer la gravité sur le corps
void Body::computeGravity()
{
    if (isAirborne())
        m_acceleration.setY(m_acceleration.y() + GameFramework::GRAVITY);
    else
        m_acceleration.setY(qMin(0.0f, m_acceleration.y()));
}

//! Permet de récupérer la vitesse du corps
//! \returns la vitesse du corps
QPointF Body::velocity() const
{
    return m_velocity;
}

//! Permet de modifier la vitesse du corps
//! \param velocity : nouvelle vitesse du corps
void Body::setVelocity(const QPointF& velocity)
{
    m_velocity = velocity;
}

//! Permet de modifier l'accélération du corps
//! \param acceleration : nouvelle accélération du corps
void Body::setAcceleration(const QPointF& acceleration)
{
    m_acceleration = acceleration;
}
