//
// Created by Dråfølin on 12.12.2023.
//

#include "hudicon.h"

HudIcon::HudIcon(const QString& rImagePath, QGraphicsItem* pParent): Sprite(rImagePath, pParent) {
    setScale(0.5);
}
