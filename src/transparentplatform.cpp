//
// Created by Dråfølin on 05.12.2023.
//

#include "transparentplatform.h"
#include "body.h"

TransparentPlatform::TransparentPlatform(QRect rect): Platform(rect)
{
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::red);
    setPixmap(QPixmap::fromImage(*m_pImage));
}

GameFramework::Direction TransparentPlatform::collisionSide(Body* body) const
{
    auto detected = Platform::collisionSide(body);
    if(detected != GameFramework::UP)
        return GameFramework::NEUTRAL;

    // Relatif à la plateforme
    int overlapTop = body->bottom() - top();
    return overlapTop > 5 ?
        GameFramework::NEUTRAL :
        GameFramework::UP;

}
