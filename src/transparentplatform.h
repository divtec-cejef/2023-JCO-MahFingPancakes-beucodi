//
// Created by Dråfølin on 05.12.2023.
//

#ifndef TRANSPARENTPLATFORM_H
#define TRANSPARENTPLATFORM_H

#include "platform.h"
#include "utilities.h"

class TransparentPlatform final : public Platform {
public:
    explicit TransparentPlatform(QRect rect);

    GameFramework::Direction collisionSide(Body *body) override;
};


#endif //TRANSPARENTPLATFORM_H
