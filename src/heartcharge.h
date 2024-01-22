//
// Created by Dråfølin on 18.01.2024.
//

#ifndef MAHFINGPANCAKES_HEARTCHARGE_H
#define MAHFINGPANCAKES_HEARTCHARGE_H


#include "item.h"

//! \brief Un objet qui permet de rajouter un emplacement de coeur au joueur
class HeartCharge : public Item {
public:
    explicit HeartCharge(QString id);
};


#endif //MAHFINGPANCAKES_HEARTCHARGE_H
