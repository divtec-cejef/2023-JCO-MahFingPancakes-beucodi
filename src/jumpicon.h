//
// Created by Dråfølin on 12.12.2023.
//

#ifndef JUMPICON_H
#define JUMPICON_H

#include "hudicon.h"

//! \brief Classe représentant une icône de "double-saut" dans l'ATH
//!
//! Tant que le joueur possède au moins un JumpCharge, il peut sauter,
//! même s'il est en l'air.
class JumpIcon final : public HudIcon {
public:
    explicit JumpIcon(QGraphicsItem *pParent = nullptr);
};


#endif //JUMPICON_H
