//
// Created by Dråfølin on 05.12.2023.
//

#include "fallingplatform.h"

FallingPlatform::FallingPlatform(QRect rect): Platform(rect)
{
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::yellow);
    setPixmap(QPixmap::fromImage(*m_pImage));
}

void FallingPlatform::tick(long long elapsedTimeInMilliseconds)
{
    if (m_isFalling) {
        m_fallSpeed += GameFramework::meterToPx(GameFramework::GRAVITY) * elapsedTimeInMilliseconds / 1000;
        setY(y() + m_fallSpeed * elapsedTimeInMilliseconds / 1000);
    }
}

GameFramework::Direction FallingPlatform::collisionSide(Body* body)
{
    auto collisionSide = Platform::collisionSide(body);
    if (collisionSide == GameFramework::Direction::UP) {
        m_isFalling = true;
    }
    return collisionSide;
}
