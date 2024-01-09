//
// Created by Dråfølin on 20.12.2023.
//

#include "hearticon.h"

#include "resources.h"

HeartIcon::HeartIcon(QGraphicsItem* pParent): HudIcon(GameFramework::imagesPath() + "/heart.png", pParent)
{
    qDebug() << "HeartIcon created";
    setScale(SCALE / sceneBoundingRect().width());
}
