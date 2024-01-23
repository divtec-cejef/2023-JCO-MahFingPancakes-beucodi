//
// Created by Dråfølin on 22.01.2024.
//

#include "finalboss.h"

//! Constructeur de la classe FinalBoss
//! \param pos : position initiale du boss
FinalBoss::FinalBoss(QPoint pos) : JumpingPancake(pos) {
    setScale(1);
    m_damage = 2;
    m_maxHealth = 10;
}
