//
// Created by Dråfølin on 20.12.2023.
//

#include "enemy.h"

#include <QRandomGenerator64>

#include "player.h"

//! Constructeur de base de la classe Enemy.
//! \param rImagePath : le chemin vers l'image de l'ennemi
//! \param pParent : pointeur vers l'objet parent
Enemy::Enemy(const QString& rImagePath, QPoint pos, QGraphicsItem* pParent)
    : Entity(rImagePath, pParent)
{
    setPos(pos);
}

//! Liaison du joueur
//! \param pPlayer : pointeur vers le joueur
void Enemy::linkPlayer(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}

//! Permet d'obtenir les dégâts infligés par l'ennemi
int Enemy::getDamage() const
{
    return m_damage;
}

//! Cadance de l'ennemi
//! \param elapsedTimeInMilliseconds : temps écoulé depuis le dernier appel de cette fonction
void Enemy::tick(const long long elapsedTimeInMilliseconds)
{
    Entity::tick(elapsedTimeInMilliseconds);
    if (m_moveCooldown > 0)
        m_moveCooldown -= static_cast<int>(elapsedTimeInMilliseconds);
    else if (!isAirborne())
    {
        if (!m_hasAttacked)
        {
            moveTowardPlayer();
            m_hasAttacked = true;
        }
    }
    else
    {
        m_moveCooldown = QRandomGenerator64::global()->bounded(MIN_MOVE_COOLDOWN, MAX_MOVE_COOLDOWN);
        m_hasAttacked = false;
    }
}
