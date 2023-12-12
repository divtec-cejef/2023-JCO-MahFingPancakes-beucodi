//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "body.h"

class Platform;

class Player: public Body {
public:
    Player();
    void jump();
    void tick(long long elapsedTimeInMilliseconds);

private:
    void collideWithPlatform(Platform* platform);

    QPointF m_playerInput = QPointF(0, 0);
    QList<int> m_keysPressed = {};
    int m_maxJumpCharges = 12;
    int m_jumpCharges = m_maxJumpCharges;
    bool m_hasReleasedJump = true;

    const qreal MAX_SPEED_X = 5;
    //! Accélération du joueur, en mètre par seconde au carré.
    const qreal PLAYER_ACCELERATION = 12;
    //! Vitesse de saut du joueur, en mètre par seconde.
    const qreal PLAYER_JUMP_FORCE = -7;
    //! Frottement du joueur, en mètre par seconde au carré.
    const qreal PLAYER_FRICTION = 0.5;

public slots:
    void keyPressed(int key);
    void keyReleased(int key);
};



#endif //PLAYER_H
