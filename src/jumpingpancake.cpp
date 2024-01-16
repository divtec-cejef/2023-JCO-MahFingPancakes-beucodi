//
// Created by Dråfølin on 09.01.2024.
//

#include "jumpingpancake.h"

#include "player.h"
#include "resources.h"

#include "body.h"

//! Constructeur de la classe JumpingPancake
//! \param pos : Position initiale de l'ennemi
//! \param pParent : pointeur vers l'objet parent
JumpingPancake::JumpingPancake(const QPoint pos, QGraphicsItem* pParent)
    : Enemy(QString("%1/pancake/idle.png").arg(GameFramework::imagesPath()), pos, pParent)
{
    m_maxHealth = 1;
    m_damage = 1;
    setScale(ENNEMI_WIDTH / sceneBoundingRect().width());

    addAnimationFrame(QString("%1/pancake/idle.png").arg(GameFramework::imagesPath()));
    for (int i = 1; i <= 3; ++i)
    {
        addAnimationFrame(QString("%1/pancake/jmp%2.png").arg(GameFramework::imagesPath()).arg(i));
    }
    setCurrentAnimationFrame(0);
}

//! Permet de mettre à jour le JumpingPancake et d'appliquer ses physiques
void JumpingPancake::tick(long long elapsedTimeInMilliseconds)
{
    Enemy::tick(elapsedTimeInMilliseconds);
    computeGravity();
    if (!isAirborne())
        m_velocity = {0, 0};
}

//! Fait sauter l'ennemi
void JumpingPancake::moveTowardPlayer()
{
    if (!isAirborne())
        jumpWithAnimation();
}

//! Permet de mettre à jour l'animation de saut
void JumpingPancake::jumpWithAnimation()
{
    if (m_jumpAnimationFrame >= 7)
    {
        m_jumpAnimationFrame = 0;
        setCurrentAnimationFrame(0);
        const qreal lateralForce = qMax(-MAX_SPEED_X,
                                        qMin(MAX_SPEED_X, (m_pPlayer->x() - x()) / DISTANCE_TO_PLAYER_RATIO));
        m_velocity.setX(lateralForce);

        m_velocity.setY(JUMP_FORCE);
        return;
    }

    setCurrentAnimationFrame(m_jumpAnimationFrame > 4 ? 7 - m_jumpAnimationFrame : m_jumpAnimationFrame);
    ++m_jumpAnimationFrame;
    QTimer::singleShot(50, [this] { this->jumpWithAnimation(); });
}
