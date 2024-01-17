//
// Created by Dråfølin on 12.12.2023.
//

#include "jumpicon.h"

#include "resources.h"

JumpIcon::JumpIcon(QGraphicsItem *pParent) : HudIcon(GameFramework::imagesPath() + "/jumpcharge.png", pParent) {
    setPos(0, 0);
    setScale(.1);
    setZValue(8);
}
