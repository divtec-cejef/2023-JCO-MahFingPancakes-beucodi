//
// Created by Dråfølin on 09.01.2024.
//

#include "jumpingpancake.h"

#include "player.h"
#include "resources.h"

//! Constructeur de la classe JumpingPancake
//! \param pPlayer : pointeur vers le joueur
//! \param pParent : pointeur vers l'objet parent
JumpingPancake::JumpingPancake(Player* pPlayer, QGraphicsItem* pParent)
    : Enemy(GameFramework::imagesPath() + "/pancake/idle.png", pPlayer, pParent)
{
    m_maxHealth = 1;
    m_damage = 1;
    setScale(ENNEMI_WIDTH / sceneBoundingRect().width());
}

//! Permet de mettre à jour le JumpingPancake et d'appliquer ses physiques
void JumpingPancake::tick(long long elapsedTimeInMilliseconds)
{
    Enemy::tick(elapsedTimeInMilliseconds);
    computeGravity();
    if (!isAirborne())
        m_velocity = {0, 0};
}

//! Permet de planifier le prochain mouvement
void JumpingPancake::moveTowardPlayer()
{
    if (isAirborne())
        return;

    const qreal lateralForce = qMax(-MAX_SPEED_X, qMin(MAX_SPEED_X, (m_pPlayer->x() - x()) / DISTANCE_TO_PLAYER_RATIO));
    m_velocity.setX(lateralForce);

    m_velocity.setY(JUMP_FORCE);
}
