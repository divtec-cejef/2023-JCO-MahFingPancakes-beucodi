//
// Created by Dråfølin on 14.11.2023.
//

#include "platform.h"
#include "sprite.h"
#include "body.h"

//! Constructeur d'une plate-forme.
//! \param rect Rectangle de la plate-forme.
//! \note Une plate-forme est une classe abstraite, il faut donc utiliser une de ses classes filles.
Platform::Platform(const QRect rect) {
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    setPos(rect.x(), rect.y());
}

/**
* Retourne la direction dans la quelle le joueur touche la plate-forme, du point de vue de celle-ci
* @returns la direction
*/
GameFramework::Direction Platform::collisionSide(Body *body) {
    // Relatif à la plateforme
    const int overlapLeft = body->right() - left();
    const int overlapRight = right() - body->left();
    const int overlapTop = body->bottom() - top();
    const int overlapBottom = bottom() - body->top();

    if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom && overlapTop > 1)
        return GameFramework::LEFT;
    if (overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom && overlapTop > 1)
        return GameFramework::RIGHT;
    if (overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom)
        return GameFramework::UP;
    if (overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop)
        return GameFramework::DOWN;
    return GameFramework::NEUTRAL;
}

//! Destructeur de la plate-forme.
Platform::~Platform() {
    delete m_pImage;
}
