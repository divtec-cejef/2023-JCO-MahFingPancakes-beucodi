//
// Created by Dråfølin on 20.12.2023.
//

#include "enemy.h"
#include "player.h"

//! Constructeur de base de la classe Enemy.
//! \param rImagePath : le chemin vers l'image de l'ennemi
//! \param pPlayer : pointeur vers le joueur
//! \param pParent : pointeur vers l'objet parent
Enemy::Enemy(const QString& rImagePath, Player* pPlayer, QGraphicsItem* pParent)
    : Entity(rImagePath, pParent)
{
    m_pPlayer = pPlayer;
}

//! Permet de faire bouger l'ennemi vers le joueur
void Enemy::moveTowardPlayer()
{
    if (m_pPlayer->x() < x())
        m_velocity.setX(-MAX_SPEED_X);
    else if (m_pPlayer->x() > x())
        m_velocity.setX(MAX_SPEED_X);
    else
        m_velocity.setX(0);
    m_velocity.setY(JUMP_FORCE);
}

//! Cadance de l'ennemi
//! \param elapsedTimeInMilliseconds : temps écoulé depuis le dernier appel de cette fonction
void Enemy::tick(const long long elapsedTimeInMilliseconds)
{
    Body::tick(elapsedTimeInMilliseconds);
}
