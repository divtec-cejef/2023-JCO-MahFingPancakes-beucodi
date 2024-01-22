//
// Created by Dråfølin on 12.12.2023.
//

#ifndef HUDICON_H
#define HUDICON_H

#include "sprite.h"

//! \brief Classe générique pour les icônes de l'ATH
class HudIcon : public Sprite {
protected:
    explicit HudIcon(const QString &rImagePath, QGraphicsItem *pParent = nullptr);
};


#endif //HUDICON_H
