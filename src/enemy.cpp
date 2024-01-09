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

//! Initialisation de l'ennemi
void Enemy::init()
{
    Entity::init();
    planMovement();
}

//! Cadance de l'ennemi
//! \param elapsedTimeInMilliseconds : temps écoulé depuis le dernier appel de cette fonction
void Enemy::tick(const long long elapsedTimeInMilliseconds)
{
    Entity::tick(elapsedTimeInMilliseconds);
}

//! Permet de planifier le prochain mouvement
void Enemy::planMovement()
{
    QTimer::singleShot(1000, this, &Enemy::planMovement);
    moveTowardPlayer();
}
