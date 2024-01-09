//
// Created by Dråfølin on 20.12.2023.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Player;

//! La clase Enemy est une classe abstraite qui représente n'importe quel ennemi dans le jeu.
//! Elle doit être héritée pour être utilisée.
class Enemy : public Entity
{
public:
    void tick(long long elapsedTimeInMilliseconds) override;

protected:
    explicit Enemy(const QString& rImagePath, Player* pPlayer, QGraphicsItem* pParent = nullptr);
    //! Pointeur vers le joueur.
    Player* m_pPlayer = nullptr;
    void moveTowardPlayer();
    //! Dégâts infligés par l'ennemi.
    const int m_damage = 0;
};


#endif //ENEMY_H
