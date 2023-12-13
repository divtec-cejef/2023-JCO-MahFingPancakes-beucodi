//
// Created by Dråfølin on 12.12.2023.
//

#ifndef FRAGILEPLATFORM_H
#define FRAGILEPLATFORM_H

#include "platform.h"

class FragilePlatform final : public Platform
{
public:
    explicit FragilePlatform(QRect rect);
    GameFramework::Direction collisionSide(Body* body) override;
    void tick(long long elapsedTimeInMilliseconds) override;

private:
    bool m_isBreaking = false;
    int m_breakingState = 255;
};


#endif //FRAGILEPLATFORM_H
