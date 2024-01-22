//
// Created by Dråfølin on 05.12.2023.
//

#ifndef TRANSPARENTPLATFORM_H
#define TRANSPARENTPLATFORM_H

#include "platform.h"
#include "utilities.h"

//! \brief Classe représentant une plateforme à sens unique
//!
//! Les plateformes à sens unique sont des plateformes sur lesquelles le joueur peut marcher.
//! Il peut aussi les traverser par le dessous, mais pas par le dessus.
class TransparentPlatform final : public Platform {
public:
    explicit TransparentPlatform(QRect rect);

    GameFramework::Direction collisionSide(Body *body) override;
};


#endif //TRANSPARENTPLATFORM_H
