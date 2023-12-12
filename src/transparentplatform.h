//
// Created by Dråfølin on 05.12.2023.
//

#ifndef TRANSPARENTPLATFORM_H
#define TRANSPARENTPLATFORM_H

#include "platform.h"
#include "utilities.h"

class TransparentPlatform: public Platform {
public:
    TransparentPlatform(QRect rect);
    GameFramework::Direction collisionSide(Body* body) const override;
};



#endif //TRANSPARENTPLATFORM_H
