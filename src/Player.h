//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

class Platform;

class Player: public Sprite {
public:
    Player();
    void jump();
    void tick(qreal deltaMs);
    bool isAirborne() const;

private:
    void collideWithPlatform(Platform* platform);

    QPointF m_velocity = QPointF(0, 0);
    QPointF m_playerInput = QPointF(0, 0);
    QList<int> m_keysPressed = {};
    int m_maxJumpCharges = 12;
    int m_jumpCharges = m_maxJumpCharges;
    bool m_hasReleasedJump = true;

    //! Vitesse du joueur, en mètre par seconde.
    const qreal PLAYER_SPEED = 5;
    //! Accélération du joueur, en mètre par seconde au carré.
    const qreal PLAYER_ACCELERATION = 7;

public slots:
    void keyPressed(int key);
    void keyReleased(int key);
};



#endif //PLAYER_H
