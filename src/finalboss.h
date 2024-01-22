//
// Created by Dråfølin on 22.01.2024.
//

#ifndef MAHFINGPANCAKES_FINALBOSS_H
#define MAHFINGPANCAKES_FINALBOSS_H


#include "jumpingpancake.h"

//! Classe représentant le boss final
class FinalBoss final : public JumpingPancake {
public:
    explicit FinalBoss(QPoint pos = {0, 0});
};


#endif //MAHFINGPANCAKES_FINALBOSS_H
