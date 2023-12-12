//
// Created by Dråfølin on 05.12.2023.
//

#ifndef FALLINGPLATFORM_H
#define FALLINGPLATFORM_H

#include "platform.h"
#include "utilities.h"

class FallingPlatform: public Platform {
public:
    FallingPlatform(QRect rect);
    void tick(long long elapsedTimeInMilliseconds) override;
    GameFramework::Direction collisionSide(Body* body) override;

private:
    bool m_isFalling = false;
    qreal m_fallSpeed = 0;
};



#endif //FALLINGPLATFORM_H
