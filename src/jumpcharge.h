//
// Created by Dråfølin on 17.01.2024.
//

#ifndef MAHFINGPANCAKES_JUMPCHARGE_H
#define MAHFINGPANCAKES_JUMPCHARGE_H


#include "item.h"

//! \brief Classe représentant un emplacement de saut supplémentaire
//!
//! Pour chaque JummpCharge ramassé, le joueur peut effectuer un "double-saut" supplémentaire.
//! Il peut ainsi atteindre des plateformes plus hautes.
class JumpCharge final : public Item {
public:
    explicit JumpCharge(QString id);
};


#endif //MAHFINGPANCAKES_JUMPCHARGE_H
