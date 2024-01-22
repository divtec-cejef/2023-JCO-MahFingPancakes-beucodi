//
// Created by Dråfølin on 05.12.2023.
//

#ifndef SOLIDPLATFORM_H
#define SOLIDPLATFORM_H

#include "platform.h"

//! \brief Classe représentant une plateforme solide
//!
//! Les plateformes solides sont des plateformes sur lesquelles le joueur peut marcher.
//! Elles restent immobiles, quoi qu'il arrive.
class SolidPlatform final : public Platform {
public:
    explicit SolidPlatform(QRect rect);
};


#endif //SOLIDPLATFORM_H
