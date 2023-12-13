//
// Created by Dråfølin on 05.12.2023.
//

#include "transparentplatform.h"
#include "body.h"

//! Constructeur de la classe TransparentPlatform
TransparentPlatform::TransparentPlatform(const QRect rect): Platform(rect)
{
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::red);
    setPixmap(QPixmap::fromImage(*m_pImage));
}

//! Permet de définir quel côté de la plateforme est en collision avec le corps
//! \param body : pointeur vers le corps avec lequel la plateforme est en collision
//! \return le côté de la plateforme en collision avec le corps
GameFramework::Direction TransparentPlatform::collisionSide(Body* body)
{
    if (Platform::collisionSide(body) != GameFramework::UP)
        return GameFramework::NEUTRAL;

    // Relatif à la plateforme
    const int overlapTop = body->bottom() - top();
    return overlapTop > (body->velocity().y() + 1) ? GameFramework::NEUTRAL : GameFramework::UP;
}
