//
// Created by Dråfølin on 14.11.2023.
//

#ifndef PLATFORM_H
#define PLATFORM_H
#include "sprite.h"
#include "utilities.h"

class Player;

class Platform: public Sprite {
public:
    GameFramework::Direction collisionSide(Player* player) const;
protected:
    Platform(QRect rect);
    QImage* m_pImage = nullptr;
};



#endif //PLATFORM_H
