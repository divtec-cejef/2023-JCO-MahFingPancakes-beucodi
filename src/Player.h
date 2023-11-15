//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"

class Player: public Sprite {
public:
    Player();
    void moveLeft(qreal deltaMs);
    void moveRight(qreal deltaMs);
    void jump(bool longJump = false);
    void tick(qreal deltaMs);
    bool isAirborne() const;

private:
    QPointF m_velocity = QPointF(0, 0);
    QList<int> m_keysPressed = {};
    int m_longJumpCooldown = 0;

public slots:
    void keyPressed(int key);
    void keyReleased(int key);
};



#endif //PLAYER_H
