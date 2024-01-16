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
    Q_OBJECT

public:
    void tick(long long elapsedTimeInMilliseconds) override;
    [[nodiscard]] int getDamage() const;
    void linkPlayer(Player* pPlayer);

protected:
    explicit Enemy(const QString& rImagePath, QPoint pos = {0, 0}, QGraphicsItem* pParent = nullptr);
    //! Pointeur vers le joueur.
    Player* m_pPlayer = nullptr;
    //! Dégâts infligés par l'ennemi.
    int m_damage = 0;

public slots:
    virtual void moveTowardPlayer() = 0;

private:
    static constexpr int MIN_MOVE_COOLDOWN = 1000;
    static constexpr int MAX_MOVE_COOLDOWN = 3000;
    int m_moveCooldown = 0;
    bool m_hasAttacked = false;

signals:
};


#endif //ENEMY_H
