//
// Created by Dråfølin on 09.01.2024.
//

#ifndef FLYINGPANCAKE_H
#define FLYINGPANCAKE_H

#include "enemy.h"

//! \brief Classe représentant un ennemi qui vole 
class FlyingPancake : public Enemy {
public:
    explicit FlyingPancake(QPoint pos = {0, 0}, QGraphicsItem *pParent = nullptr);

    void tick(long long elapsedTimeInMilliseconds) override;

    void moveTowardPlayer() override;

protected:
    //! Largeur de l'ennemi, en px
    constexpr static qreal ENNEMI_WIDTH = 60;
    //! Vitesse de l'ennemi, en mètres par seconde.
    constexpr static qreal SPEED = 4;
    //! Rapport de réduction de la vitesse de l'ennemi par rapport a la distance du joueur
    constexpr static qreal DISTANCE_TO_PLAYER_RATIO = 50;

    [[nodiscard]] int getMinCooldown() override {
        return 1;
    }

    [[nodiscard]] int getMaxCooldown() override {
        return 1;
    }
};


#endif //FLYINGPANCAKE_H
