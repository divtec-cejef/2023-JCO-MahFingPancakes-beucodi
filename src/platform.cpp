//
// Created by Dråfølin on 14.11.2023.
//

#include "platform.h"

#include <resources.h>

#include "sprite.h"
#include "player.h"

Platform::Platform(QRect rect) {
    m_pImage = new QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
    setPos(rect.x(), rect.y());
}

/**
* Retourne la direction dans la quelle le joueur touche la plate-forme, du point de vue de celle-ci
* @returns la direction
*/
GameFramework::Direction Platform::collisionSide(Player* player) const
{
    // Relatif à la plateforme
    int overlapLeft = player->right() - left();
    int overlapRight = right() - player->left();
    int overlapTop = player->bottom() - top();
    int overlapBottom = bottom() - player->top();

    return overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom ?
        GameFramework::LEFT:
    overlapRight < overlapLeft && overlapRight < overlapTop && overlapRight < overlapBottom?
        GameFramework::RIGHT:
    overlapTop < overlapLeft && overlapTop < overlapRight && overlapTop < overlapBottom?
        GameFramework::UP:
    overlapBottom < overlapLeft && overlapBottom < overlapRight && overlapBottom < overlapTop?
        GameFramework::DOWN:
    GameFramework::NEUTRAL;
}
