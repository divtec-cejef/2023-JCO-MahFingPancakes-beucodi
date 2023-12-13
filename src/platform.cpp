//
// Created by Dråfølin on 14.11.2023.
//

#include "platform.h"
#include "sprite.h"
#include "body.h"

//! Constructeur d'une plate-forme.
//! \param rect Rectangle de la plate-forme.
//! \note Une plate-forme est une classe abstraite, il faut donc utiliser une de ses classes filles.
Platform::Platform(const QRect rect)
{
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    setPos(rect.x(), rect.y());
}

/**
* Retourne la direction dans la quelle le joueur touche la plate-forme, du point de vue de celle-ci
* @returns la direction
*/
GameFramework::Direction Platform::collisionSide(Body* body)
{
    // Relatif à la plateforme
    const int overlapLeft = body->right() - left();
    const int overlapRight = right() - body->left();
    const int overlapTop = body->bottom() - top();
    const int overlapBottom = bottom() - body->top();

    return overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom
               ? GameFramework::LEFT
               : overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom
               ? GameFramework::RIGHT
               : overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom
               ? GameFramework::UP
               : overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop
               ? GameFramework::DOWN
               : GameFramework::NEUTRAL;
}
