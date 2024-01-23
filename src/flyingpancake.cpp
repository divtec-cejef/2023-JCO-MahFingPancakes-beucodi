//
// Created by Dråfølin on 09.01.2024.
//

#include "flyingpancake.h"

#include <QThread>

#include "player.h"
#include "resources.h"

#include "body.h"

//! Constructeur de la classe FlyingPancake
//! \param pos : Position initiale de l'ennemi
//! \param pParent : pointeur vers l'objet parent
FlyingPancake::FlyingPancake(const QPoint pos, QGraphicsItem *pParent)
        : Enemy(QString("%1/flyingpancake.png").arg(GameFramework::imagesPath()), pos, pParent) {
    m_maxHealth = 3;
    m_damage = 1;
    setScale(ENNEMI_WIDTH / sceneBoundingRect().width());
}

//! Permet de mettre à jour le FlyingPancake et d'appliquer ses physiques
void FlyingPancake::tick(long long elapsedTimeInMilliseconds) {
    QPointF toPlayerVector = {
	    m_pPlayer->x() - x(),
	    m_pPlayer->y() - y()
    };

    auto magnitude = qSqrt(qPow(toPlayerVector.x(), 2.0) + qPow(toPlayerVector.y(), 2.0)); 

    toPlayerVector = {
	toPlayerVector.x() / magnitude * SPEED,
	toPlayerVector.y() / magnitude * SPEED
    };

    moveBy(toPlayerVector.x(), toPlayerVector.y());

    Enemy::tick(elapsedTimeInMilliseconds);
}

//! Fait voler l'ennemi vers le joueur
void FlyingPancake::moveTowardPlayer() {
}
