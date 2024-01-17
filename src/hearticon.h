//
// Created by Dråfølin on 20.12.2023.
//

#ifndef HEARTICON_H
#define HEARTICON_H

#include "hudicon.h"

class HeartIcon : public HudIcon {
public:
    explicit HeartIcon(QGraphicsItem *pParent = nullptr);

    constexpr static qreal SCALE = 7.0;
};


#endif //HEARTICON_H
