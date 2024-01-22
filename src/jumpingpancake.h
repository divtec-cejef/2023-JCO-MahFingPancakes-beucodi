//
// Created by Dråfølin on 09.01.2024.
//

#ifndef JUMPINGPANCAKE_H
#define JUMPINGPANCAKE_H

#include "enemy.h"

//! Classe représentant un ennemi qui saute
class JumpingPancake : public Enemy {
public:
    explicit JumpingPancake(QPoint pos = {0, 0}, QGraphicsItem *pParent = nullptr);

    void tick(long long elapsedTimeInMilliseconds) override;

    void moveTowardPlayer() override;

protected:
    //! Force de saut de l'entité, en mètre par seconde.
    //! \note la force est appliquée en une seule fois, et non pas en continu.
    //! La force est déja en négatif, ce qui evite d'inverser dans le setX
    constexpr static qreal JUMP_FORCE = -5;
    //! Largeur de l'ennemi, en px
    constexpr static qreal ENNEMI_WIDTH = 60;
    //! Vitesse horizontale maximale de l'ennemi, en mètres par seconde.
    constexpr static qreal MAX_SPEED_X = 7;
    //! Rapport de réduction de la vitesse de l'ennemi par rapport a la distance du joueur
    constexpr static qreal DISTANCE_TO_PLAYER_RATIO = 50;

    [[nodiscard]] int getMinCooldown() override {
        return 100;
    }

    [[nodiscard]] int getMaxCooldown() override {
        return 500;
    }

private:
    //! Compteur d'images pour l'animation de saut.
    //! \note Le joueur saute quand le compteur atteint 7
    int m_jumpAnimationFrame = 0;
    //! Cooldown entre les images de l'animation de saut
    int m_jumpAnimationCooldown = 0;

    void jumpWithAnimation();
};


#endif //JUMPINGPANCAKE_H
