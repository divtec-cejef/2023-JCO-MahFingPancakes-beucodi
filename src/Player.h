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
    void jump();
    void tick(qreal deltaMs);
    bool isAirborne() const;

private:
    QPointF m_velocity = QPointF(0, 0);
};



#endif //PLAYER_H
