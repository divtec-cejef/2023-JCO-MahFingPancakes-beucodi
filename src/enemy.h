//
// Created by Dråfølin on 20.12.2023.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Player;

//! La clase Enemy est une classe abstraite qui représente n'importe quel ennemi dans le jeu.
//! Elle doit être héritée pour être utilisée.
class Enemy : public Entity {
Q_OBJECT

public:
    void tick(long long elapsedTimeInMilliseconds) override;

    [[nodiscard]] int getDamage() const;

    void linkPlayer(Player *pPlayer);

public slots:

    virtual void moveTowardPlayer() = 0;

protected:
    explicit Enemy(const QString &rImagePath, QPoint pos = {0, 0}, QGraphicsItem *pParent = nullptr);

    //! Pointeur vers le joueur.
    Player *m_pPlayer = nullptr;
    //! Dégâts infligés par l'ennemi.
    int m_damage = 0;
    
    void die() override;

    [[nodiscard]] virtual int getMinCooldown() {
        return 1000;
    }

    [[nodiscard]] virtual int getMaxCooldown() {
        return 3000;
    }

private:
    int m_moveCooldown = 0;
    bool m_hasAttacked = false;

signals:
};


#endif //ENEMY_H
