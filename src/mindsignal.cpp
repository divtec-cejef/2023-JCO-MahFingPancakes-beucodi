//
// Created by Dråfølin on 16.01.2024.
//

#include "mindsignal.h"

#include "enemy.h"
#include "gamescene.h"
#include "resources.h"
#include "utilities.h"

//! Constructeur de MindSignal
//! \param pParent le parent du sprite
MindSignal::MindSignal(QGraphicsItem* pParent): Sprite(GameFramework::imagesPath() + "/MindSignal.png", pParent)
{
    setScale(GameFramework::meterToPx(ATTACK_SIZE) / width());
    setOffset(-boundingRect().width() / 2, -boundingRect().height() / 2);
}

//! Permet de mettre a jour la direction du signal
void MindSignal::setDirection(QPointF goalPos)
{
    auto currentPos = scenePos();
    currentPos.setX(currentPos.x() + sceneBoundingRect().width() / 2);
    currentPos.setY(currentPos.y() + sceneBoundingRect().height() / 2);
    m_deltaPos.setX(goalPos.x() - currentPos.x());
    m_deltaPos.setY(goalPos.y() - currentPos.y());
    // On normalise le vecteur
    m_deltaPos /= sqrt(m_deltaPos.x() * m_deltaPos.x() + m_deltaPos.y() * m_deltaPos.y());

    setRotation(atan2(m_deltaPos.y(), m_deltaPos.x()) * 180 / M_PI);
}

void MindSignal::tick(long long elapsedTimeInMilliseconds)
{
    Sprite::tick(elapsedTimeInMilliseconds);
    moveBy(
        m_deltaPos.x() * (static_cast<qreal>(elapsedTimeInMilliseconds) / 1000.0) * GameFramework::meterToPx(
            ATTACK_SPREAD_SPEED),
        m_deltaPos.y() * (static_cast<qreal>(elapsedTimeInMilliseconds) / 1000.0) * GameFramework::meterToPx(
            ATTACK_SPREAD_SPEED)
    );

    if (x() < 0 || x() > m_pParentScene->width() || y() < 0 || y() > m_pParentScene->height())
    {
        emit queueForDeletion(this);
    }

    for(const auto sprite: collidingSprites(sceneBoundingRect()))
    {
        if(const auto enemy = dynamic_cast<Enemy*>(sprite))
        {
            enemy->takeDamage(ATTACK_POWER);
        emit queueForDeletion(this);
        break;
        }
    }
}
