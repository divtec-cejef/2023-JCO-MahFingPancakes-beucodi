//
// Created by Dråfølin on 12.12.2023.
//

#include "fragileplatform.h"

#include <gamescene.h>

#include "body.h"

//! Constructeur de la classe FragilePlatform
//! \param rect rectangle de la plateforme
FragilePlatform::FragilePlatform(QRect rect): Platform(rect)
{
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    m_pImage->fill(Qt::cyan);
    setPixmap(QPixmap::fromImage(*m_pImage));
}

//! Retourne la direction dans la quelle le joueur touche la plate-forme, du point de vue de celle-ci
//! Active en plus le fait que la plateforme se brise
//! \param body le joueur
GameFramework::Direction FragilePlatform::collisionSide(Body* body)
{
    auto detected = Platform::collisionSide(body);
    if(detected != GameFramework::UP)
        return detected;

    m_isBreaking = true;

    return GameFramework::NEUTRAL;
}

//! Met à jour la plateforme
//! \param elapsedTimeInMilliseconds temps écoulé depuis la dernière mise à jour
void FragilePlatform::tick(long long elapsedTimeInMilliseconds)
{
    if(m_isBreaking)
    {
        m_pImage->fill(QColor(0, 255, 255, m_breakingState));
        setPixmap(QPixmap::fromImage(*m_pImage));
        m_breakingState-=20;
    }

    if(m_breakingState <= 0)
    {
        m_isBreaking = false;
        emit queuedForDeletion(this);
    }
}
