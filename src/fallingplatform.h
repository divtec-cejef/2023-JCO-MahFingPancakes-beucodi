//
// Created by Dråfølin on 05.12.2023.
//

#ifndef FALLINGPLATFORM_H
#define FALLINGPLATFORM_H

#include "platform.h"
#include "utilities.h"

//! \brief Classe de platforme spécifique qui tombe lorsqu'un joueur se tient dessus
//!
//! Cette classe hérite de la classe Platform et implémente la méthode tick() pour faire tomber la plateforme lorsqu'un joueur se tient dessus.
class FallingPlatform final : public Platform {
public:
    explicit FallingPlatform(QRect rect);

    void tick(long long elapsedTimeInMilliseconds) override;

    GameFramework::Direction collisionSide(Body *body) override;

private:
    bool m_isFalling = false;
    qreal m_fallSpeed = 0;
};


#endif //FALLINGPLATFORM_H
