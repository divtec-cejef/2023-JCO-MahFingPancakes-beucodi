//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLATFORM_H
#define PLATFORM_H
#include "sprite.h"
#include "utilities.h"

class Body;

class Platform: public Sprite {
public:
    virtual GameFramework::Direction collisionSide(Body* body);
protected:
    Platform(QRect rect);
    QImage* m_pImage = nullptr;
};



#endif //PLATFORM_H
